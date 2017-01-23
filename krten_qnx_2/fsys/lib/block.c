
/*
 *  block.c
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
 *  This module contains block functions that deal with the data storage
 *  blocks.
 *
 *  2002 02 28  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "cfs.h"

// hack!  grab -m -M values from main.c
extern  int     optm;
extern  int     optM;
extern  mpool_t *mpool_block;               // (main.c) global (sigh) -- move this elsewhere when we do the mount logic

void *
cfs_block_alloc (cfs_attr_t *attr)
{
    return (mpool_calloc (mpool_block));
}

void
cfs_block_free (cfs_attr_t *attr, void *ptr)
{
    mpool_free (mpool_block, ptr);
}

void
cfs_block_fill_statvfs (cfs_attr_t *attr, struct statvfs *r)
{
    uint32_t            nalloc, nfree;
    size_t              nbytes;

    mpool_info (mpool_block, &nbytes, &r -> f_blocks, &nalloc, &nfree, NULL, NULL);

    // invariant
    r -> f_bsize = nbytes;                                                      // file system block size
    r -> f_frsize = nbytes;                                                     // fundamental filesystem block size
    r -> f_files = INT_MAX;                                                     // total number of file serial numbers
    r -> f_fsid = 0x12345678;                                                   // file system id
    r -> f_flag = 0;                                                            // bit mask of f_flag values
    r -> f_namemax = NAME_MAX;                                                  // maximum filename length
    strcpy (r -> f_basetype, "cfs");                                            // null terminated name of target file system

    // calculated
    if (optm) {                                                                 // for system-allocated memory with a maximum
        r -> f_blocks = optm / nbytes;                                          // total number of blocks on file system in units of f_frsize
        r -> f_bfree = r -> f_blocks - nalloc;                                  // total number of free blocks
        r -> f_ffree = r -> f_files - nalloc;                                   // total number of free file serial numbers (an approximation)
    } else if (optM) {                                                          // for statically-allocated memory with a maximum
        r -> f_blocks = optM / nbytes;                                          // total number of blocks on file system in units of f_frsize
        r -> f_bfree = nfree;                                                   // total number of free blocks
        r -> f_ffree = nfree;                                                   // total number of free file serial numbers (an approximation)
    } else {                                                                    // for unbounded system-allocated memory
        r -> f_blocks = nalloc + 1;                                             // total number of blocks on file system in units of f_frsize
        r -> f_bfree = r -> f_blocks - nalloc;                                  // total number of free blocks
        r -> f_ffree = r -> f_files - nalloc;                                   // total number of free file serial numbers (an approximation)
    }

    // mirror
    r -> f_bavail = r -> f_bfree;                                               // number of free blocks available to non-privileged process
    r -> f_favail = r -> f_ffree;                                               // number of file serial numbers available to non-privileged process

D   printf ("cfs_block_fill_statvfs dump:\n");
D   printf ("\tf_bsize      %10ld (file system block size)\n", r -> f_bsize);
D   printf ("\tf_frsize     %10ld (fundamental filesystem block size)\n", r -> f_frsize);
D   printf ("\tf_files      %10d (total number of file serial numbers)\n", r -> f_files);
D   printf ("\tf_fsid       0x%08lX (file system id)\n", r -> f_fsid);
D   printf ("\tf_flag       0x%08lX (bit mask of f_flag values)\n", r -> f_flag);
D   printf ("\tf_namemax    %10ld (maximum filename length)\n", r -> f_namemax);
D   printf ("\tf_basetype   %10.10s (null terminated name of target file system)\n", r -> f_basetype);
D   printf ("\tf_blocks     %10d (total number of blocks on file system in units of f_frsize)\n", r -> f_blocks);
D   printf ("\tf_bfree      %10d (total number of free blocks)\n", r -> f_bfree);
D   printf ("\tf_ffree      %10d (total number of free file serial numbers)\n", r -> f_ffree);
D   printf ("\tf_bavail     %10d (number of free blocks available to non-privileged process)\n", r -> f_bavail);
D   printf ("\tf_favail     %10d (number of file serial numbers available to non-privileged process)\n", r -> f_favail);
D   printf ("\toptm         %10d [extension; command line -m value]\n", optm);
D   printf ("\toptM         %10d [extension; command line -M value]\n", optM);
}

