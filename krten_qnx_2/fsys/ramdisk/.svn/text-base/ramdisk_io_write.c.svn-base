
/*
 *  ramdisk_io_write.c
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
 *  2003 01 03  R. Krten        separated from cfs
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
ramdisk_io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
    cfs_attr_t      *attr;
    int             startblock, startoffset;
    int             lastblock;
    int             nbytes, nleft;
    int             toread;
    iov_t           *newblocks;
    int             i;
    off_t           newsize;
    int             pool_flag;                                                  // true if we're using the mpool() functions

D   printf ("%s %d:  ramdisk_io_write (nbytes %d)\n", __FILE__, __LINE__, msg -> i.nbytes);

    // shortcuts
    nbytes = msg -> i.nbytes;
    attr = ocb -> attr;
    newsize = ocb -> offset + nbytes;

    // precalc block size constants...
    startblock = ocb -> offset / BLOCKSIZE;
    startoffset = ocb -> offset & (BLOCKSIZE - 1);
    lastblock = newsize / BLOCKSIZE;

D   printf ("%s %d:  ramdisk_io_write startblock %d startoffset %d lastblock %d nbytes %d current ocb -> offset %lld\n", __FILE__, __LINE__, startblock, startoffset, lastblock, nbytes, ocb -> offset);

    /*
     *  We're going to create an iov (called "newblocks") that's big enough
     *  to hold the entire data content (in 4k chunks) that we're going to
     *  read from the client.
     *
     *  Note that if we adjust the nbytes, below, after the cfs_a_truncate,
     *  we may be allocating more IOVs than we'll use -- this is ok.  We
     *  won't *read* more data than we expect, because our filler loop is
     *  based on "nbytes".
     *
     *  Note that if startblock and lastblock are the same, we still need
     *  at least one IOV, hence the "+ 1" below.
    */

    i = lastblock - startblock + 1;
    if (i <= 8) {
        newblocks = mpool_malloc (mpool_iov8);
        pool_flag = 1;
    } else {
        newblocks = malloc (sizeof (iov_t) * i);
        pool_flag = 0;
    }

    if (newblocks == NULL) {
D       printf ("%s %d:  ramdisk_io_write couldn't allocate %d bytes of memory for newblocks, returning ENOMEM\n", __FILE__, __LINE__, sizeof (iov_t) * (lastblock - startblock + 1));
        return (ENOMEM);
    }

    /*
     *  In filling newblocks, there are several cases to consider:
     *      - the total transfer will fit within one block
     *      - the transfer will span a block, not entirely using either
     *      - the transfer will span many blocks
     *
     *  These considerations are important when calculating the first
     *  and final block IOVs because those IOVs may not necessarily
     *  start on a 4k block boundary and run for 4k.
    */

    // calculate the first block size
    toread = BLOCKSIZE - startoffset;
    if (toread > nbytes) {
        toread = nbytes;
    }
    SETIOV (&newblocks [0], (char *) (attr -> type.fileblocks [startblock].iov_base) + startoffset, toread);
D   printf ("%s %d:  ramdisk_io_write fill block [%d] addr %p length %d offset %d\n", __FILE__, __LINE__, 0, newblocks [0].iov_base, newblocks [0].iov_len, startoffset);

    // now calculate zero or more blocks; special logic exists for a short final block
    nleft = nbytes - toread;                                                    // account for the number of bytes we just consumed in the first IOV
    for (i = 1; nleft > 0; i++) {
        if (nleft > BLOCKSIZE) {
            SETIOV (&newblocks [i], attr -> type.fileblocks [startblock + i].iov_base, BLOCKSIZE);
            nleft -= BLOCKSIZE;
        } else {
            SETIOV (&newblocks [i], attr -> type.fileblocks [startblock + i].iov_base, nleft);
            nleft = 0;
        }
D       printf ("%s %d:  ramdisk_io_write fill block [%d] addr %p length %d\n", __FILE__, __LINE__, i, newblocks [i].iov_base, newblocks [i].iov_len);
    }

    // transfer the data from the client directly into the ramdisk...
D   printf ("%s %d:  ramdisk_io_write going to do msgread (#iovs %d, offset %d)\n", __FILE__, __LINE__, i, sizeof (msg -> i));
    resmgr_msgreadv (ctp, newblocks, i, sizeof (msg -> i));
    if (pool_flag) {
        mpool_free (mpool_iov8, newblocks);
    } else {
        free (newblocks);
    }

    // use the original value of nbytes here...
    if (nbytes) {
        attr -> attr.flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_DIRTY_TIME;
        ocb -> offset += nbytes;
    }
D   printf ("%s %d:  ramdisk_io_write returning %d bytes to write()\n", __FILE__, __LINE__, nbytes);
    _IO_SET_WRITE_NBYTES (ctp, nbytes);
    return (EOK);
}

