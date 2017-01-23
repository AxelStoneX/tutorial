
/*
 *  ramdisk_io_read.c
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
 *  This module is the RAMDISK's io_read handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2003 01 03  R. Krten        separated from CFS
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
extern  mpool_t         *mpool_readdir;                                         // (main.c) global -- holds 2k readdir buffer

int
ramdisk_io_read_dir (resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{
    int     nbytes;
    int     nleft;
    struct  dirent *dp;
    char    *reply_msg;
    char    *fname;
    int     pool_flag;                                                          // true if we're using the mpool() functions

    // allocate a buffer for the reply
    if (msg -> i.nbytes <= 2048) {
        reply_msg = mpool_calloc (mpool_readdir);
        pool_flag = 1;
    } else {
        reply_msg = calloc (1, msg -> i.nbytes);
        pool_flag = 0;
    }

    if (reply_msg == NULL) {
        return (ENOMEM);
    }

    // assign output buffer
    dp = (struct dirent *) reply_msg;

    // we have "nleft" bytes left
    nleft = msg -> i.nbytes;
D   printf ("%s %d:  cfs_io_read_dir size %d ocb -> offset %lld n %d\n", __FILE__, __LINE__, nleft, ocb -> offset, ocb -> attr -> nels);
    while (ocb -> offset < ocb -> attr -> nels) {

D       printf ("%s %d:  cfs_io_read_dir preparing index %lld\n", __FILE__, __LINE__, ocb -> offset);
        // short-form for name
        fname = ocb -> attr -> type.dirblocks [ocb -> offset].name;
        if (!fname) {                                                           // if directory entry is unused, skip it
D           printf ("%s %d:  cfs_io_read_dir skipping empty entry at index %lld\n", __FILE__, __LINE__, ocb -> offset);
            ocb -> offset++;
            continue;
        }

        // see how big the result is
        nbytes = dirent_size (fname);

        // do we have room for it?
        if (nleft - nbytes >= 0) {

            // fill the dirent, and advance the dirent pointer
            dp = dirent_fill (dp, ocb -> offset + 1, ocb -> offset, fname);

            // move the OCB offset
            ocb -> offset++;

            // account for the bytes we just used up
            nleft -= nbytes;

        } else {

            // don't have any more room, stop
            break;
        }
    }

    // if we returned any entries, then update the ATIME
    if (nleft != msg -> i.nbytes) {
        ocb -> attr -> attr.flags |= IOFUNC_ATTR_ATIME | IOFUNC_ATTR_DIRTY_TIME;
    }

    // return info back to the client
    MsgReply (ctp -> rcvid, (char *) dp - reply_msg, reply_msg, (char *) dp - reply_msg);

    // release our buffer
    if (pool_flag) {
        mpool_free (mpool_readdir, reply_msg);
    } else {
        free (reply_msg);
    }

    // tell resource manager library we already did the reply
    return (_RESMGR_NOREPLY);
}

int
ramdisk_io_read_file (resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{
    int     nbytes;
    int     nleft;
    int     towrite;
    iov_t   *iovs;
    int     niovs;                                                              // use this many IOVs in the output reply buffer.
    int     startoffset;                                                        // where in a BLOCKSIZE do we start?
    int     startblock;                                                         // starting block number
    int     i;
    int     pool_flag;                                                          // true if we're using the mpool() functions

    // we don't do any xtypes here...
    if ((msg -> i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return (ENOSYS);
    }

    // figure out how many bytes are left
    nleft = ocb -> attr -> attr.nbytes - ocb -> offset;

    // and how many we can return to the client
    nbytes = min (nleft, msg -> i.nbytes);

    if (nbytes) {

        niovs = nbytes / BLOCKSIZE + 2;                                         // why +2?  We could be returning 2 bytes (2 / BLOCKSIZE == 0), with one byte in one 4k block (+1) and the other byte in another 4k block (+1)
        if (niovs <= 8) {
            iovs = mpool_malloc (mpool_iov8);
            pool_flag = 1;
        } else {
            iovs = malloc (sizeof (iov_t) * niovs);
            pool_flag = 0;
        }
        if (iovs == NULL) {
            return (ENOMEM);
        }

        startoffset = ocb -> offset & (BLOCKSIZE - 1);                          // find the start within the block
        startblock  = ocb -> offset / BLOCKSIZE;                                // figure out which block we start at...
        towrite = BLOCKSIZE - startoffset;
        if (towrite > nbytes) {
            towrite = nbytes;
        }

        SETIOV (&iovs [0], (char *) (ocb -> attr -> type.fileblocks [startblock].iov_base) + startoffset, towrite);
D       printf ("%s %d:  cfs_io_read_file fill block [%d] addr %p length %d offset %d\n", __FILE__, __LINE__, 0, iovs [0].iov_base, iovs [0].iov_len, startoffset);
        nleft = nbytes - towrite;                                               // account for the number of bytes we just consumed in the first IOV
        for (i = 1; nleft > 0; i++) {
            if (nleft > BLOCKSIZE) {
                SETIOV (&iovs [i], ocb -> attr -> type.fileblocks [startblock + i].iov_base, BLOCKSIZE);
                nleft -= BLOCKSIZE;
            } else {
                SETIOV (&iovs [i], ocb -> attr -> type.fileblocks [startblock + i].iov_base, nleft);
                nleft = 0;
            }
D           printf ("%s %d:  cfs_io_read_file fill block [%d] addr %p length %d\n", __FILE__, __LINE__, i, iovs [i].iov_base, iovs [i].iov_len);
        }

        // return it to the client
D       printf ("%s %d:  returning %d bytes, %d iovs\n", __FILE__, __LINE__, nbytes, i);
        MsgReplyv (ctp -> rcvid, nbytes, iovs, i);

        // update flags and offset
        ocb -> attr -> attr.flags |= IOFUNC_ATTR_ATIME | IOFUNC_ATTR_DIRTY_TIME;
        ocb -> offset += nbytes;
        if (pool_flag) {
            mpool_free (mpool_iov8, iovs);
        } else {
            free (iovs);
        }
    } else {
        // nothing to return, indicate End Of File
        MsgReply (ctp -> rcvid, EOK, NULL, 0);
    }

    // already done the reply ourselves
    return (_RESMGR_NOREPLY);
}

