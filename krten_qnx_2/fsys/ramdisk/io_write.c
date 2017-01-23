
/*
 *  io_write.c
 *
 *  Copyright 2002, 2003 QNX Software Systems Limited. All Rights Reserved.
 * 
 *  You must obtain a written license from and pay applicable license fees to QNX 
 *  Software Systems Limited before you may reproduce, modify or distribute this software, 
 *  or any work that includes all or part of this software.   Free development 
 *  licenses are available for evaluation and non-commercial purposes.  For more 
 *  information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 *  This file may contain contributions from others.  Please review this entire 
 *  file for other proprietary rights or license notices, as well as the QNX 
 *  Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 *  for other information.
 *
 *  This module is the RAMDISK's io_write handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <devctl.h>
#include <limits.h>

#include "cfs.h"

extern  mpool_t         *mpool_iov8;                                            // (main.c) global -- holds 8 iov_t's

/*
 *  cfs_io_write
 *
 *  For the write() implementation, I'm just thinking of having a whack of 4k cache blocks.
 *  These would be stored in the iov_t arrays.  The "nels" would indicate how many 4k cache
 *  blocks exist, and the attr -> nbytes would indicate the resource's "true size".
 *  Recall that the "nalloc" member is used for the iov data structure that holds the blocks.
 *
 *  Then to implement the write:
 *
 *      o   verify that there are enough 4k blocks in existence; if not, get more
 *          - this covers the case where we are writing only to the end of an existing block
 *          - note that in case of a "full disk" condition, we may not be able to allocate
 *            all the blocks we requested; trim the request size in this case.
 *      o   find the starting offset into the first 4k block
 *      o   find the number of contiguous blocks
 *      o   find the ending offset into the last 4k block
 *      o   construct an iov_t to match the above
 *      o   do a MsgRead()
 *
 *  In order to do the read, we therefore need the following information:
 *      o   starting block number in blocklist
 *      o   starting block offset
 *      o   total number of full and partial blocks to read
 *      o   final block size
 *
 *  This allows us the most general read of something that might be misaligned on the
 *  initial block, span zero or more contiguous intermediate "full" blocks, and then be
 *  misaligned on the final block (which could be the initial block in the case of a
 *  small write()).
 *
 *  Remember that "empty" blocks (i.e., doing an lseek past the end of the file and
 *  writing) need to be zero-filled -- this is taken care of when allocating new blocks
 *  because we specify TRUNCATE_ERASE, but we still need to take care of existing
 *  data blocks (and portions thereof).
*/

int
cfs_io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
    cfs_attr_t      *attr;
    int             i;
    off_t           newsize;

D   printf ("%s %d:  cfs_io_write (nbytes %d)\n", __FILE__, __LINE__, msg -> i.nbytes);
    if ((i = iofunc_write_verify (ctp, msg, ocb, NULL)) != EOK) {
D       printf ("%s %d:  cfs_io_write called iofunc_write_verify, returns %d (%s)\n", __FILE__, __LINE__, i, strerror (i));
        return (i);
    }

    // shortcuts
    attr = ocb -> attr;
    newsize = ocb -> offset + msg -> i.nbytes;

    // precalc block size constants...

D   printf ("%s %d:  cfs_io_write nbytes %d current ocb -> offset %lld\n", __FILE__, __LINE__, msg -> i.nbytes, ocb -> offset);

    /*
     *  If the write request will take us past the current EOF, then we
     *  will need to grow the file.  The issue is that we may not have
     *  enough disk space to do so, so we grow it as much as we can, and
     *  trim back the client's request if we couldn't grow it as much as
     *  we wanted to.
    */

    if (newsize > attr -> attr.nbytes) {                                        // we need to grow the file
        cfs_a_truncate (attr, newsize, TRUNCATE_ERASE);                         // we did not specify TRUNCATE_ALL_OR_NONE, so it will allocate as many as possible in a disk-full or near-full situation
        if (newsize > attr -> attr.nbytes) {                                    // we obviously couldn't grow it to be big enough; trim the user request
            msg -> i.nbytes = attr -> attr.nbytes - ocb -> offset;              // we're going to be writing less than originally anticipated
D           printf ("%s %d:  cfs_io_write ran out of space, so extended attr.nbytes to %lld (only)\n", __FILE__, __LINE__, attr -> attr.nbytes);
            if (!msg -> i.nbytes) {
D               printf ("%s %d:  cfs_io_write DISK FULL returning ENOSPC\n", __FILE__, __LINE__);
                return (ENOSPC);
            }
        }
    }

    return (ramdisk_io_write (ctp, msg, ocb));
}

