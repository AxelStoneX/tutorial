
/*
 *  tarfs_io_read.c
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
 *  This module is the .tar filesystem's io_read handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2003 01 03  R. Krten        separated from CFS
 *  2003 01 14  R. Krten        copied from ramdisk for tarfs
 *  2003 01 15  R. Krten        added zlib support
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
#include <sys/uio.h>
#include <zlib.h>

#include "cfs.h"

extern  mpool_t         *mpool_iov8;                                            // (main.c) global -- holds 8 iov_t's
extern  mpool_t         *mpool_readdir;                                         // (main.c) global -- holds 2k readdir buffer

int
tarfs_io_read_dir (resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
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
tarfs_io_read_file (resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{
    int     nbytes;
    int     nleft;
    iov_t   *iovs;
    int     niovs;                                                              // use this many IOVs in the output reply buffer.
    int     i;
    int     pool_flag;                                                          // true if we're using the mpool() functions
    gzFile  fd;

D   printf ("%s %d:  tarfs_io_read_file (virtual file: source=%s off=%d length=%lld)\n", __FILE__, __LINE__, ocb -> attr -> type.vfile.name, ocb -> attr -> type.vfile.off, ocb -> attr -> attr.nbytes);

    // we don't do any xtypes here...
    if ((msg -> i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return (ENOSYS);
    }

    // figure out how many bytes are left
    nleft = ocb -> attr -> attr.nbytes - ocb -> offset;

    // and how many we can return to the client
    nbytes = min (nleft, msg -> i.nbytes);

    if (nbytes) {

        if ((fd = gzopen (ocb -> attr -> type.vfile.name, "r")) == NULL) {
D           printf ("%s %d:  tarfs_io_read can't open %s errno %d (%s)\n", __FILE__, __LINE__, ocb -> attr -> type.vfile.name, errno, strerror (errno));
            return (errno);
        }
        niovs = (nbytes + BLOCKSIZE - 1) / BLOCKSIZE;                           // we will always start at offset zero in a block.
        if (niovs <= 8) {
            iovs = mpool_malloc (mpool_iov8);
            pool_flag = 1;
        } else {
            iovs = malloc (sizeof (iov_t) * niovs);
            pool_flag = 0;
        }
        if (iovs == NULL) {
            gzclose (fd);
            return (ENOMEM);
        }


        for (i = 0; i < niovs; i++) {
            SETIOV (&iovs [i], cfs_block_alloc (ocb -> attr), BLOCKSIZE);
D           printf ("%s %d:  tarfs_io_read_file allocated IOV [%d] -> %p\n", __FILE__, __LINE__, i, iovs [i].iov_base);
            if (iovs [i].iov_base == NULL) {
                for (--i ; i >= 0; i--) {
                    cfs_block_free (ocb -> attr, iovs [i].iov_base);
                }
                gzclose (fd);
                return (ENOMEM);
            }
        }

        // trim last block to correctly read last entry in a .tar file
        if (nbytes & BLOCKSIZE) {
            iovs [niovs - 1].iov_len = nbytes & BLOCKSIZE;
        }

        gzseek (fd, ocb -> attr -> type.vfile.off + ocb -> offset, SEEK_SET);
        for (i = 0; i < niovs; i++) {
            gzread (fd, iovs [i].iov_base, iovs [i].iov_len);
        }
        gzclose (fd);

        // return it to the client
D       printf ("%s %d:  returning %d bytes, %d iovs\n", __FILE__, __LINE__, nbytes, i);
        MsgReplyv (ctp -> rcvid, nbytes, iovs, i);

        // update flags and offset
        ocb -> attr -> attr.flags |= IOFUNC_ATTR_ATIME | IOFUNC_ATTR_DIRTY_TIME;
        ocb -> offset += nbytes;
        for (i = 0; i < niovs; i++) {
D           printf ("%s %d:  tarfs_io_read_file freed IOV [%d] -> %p\n", __FILE__, __LINE__, i, iovs [i].iov_base);
            cfs_block_free (ocb -> attr, iovs [i].iov_base);
        }
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

