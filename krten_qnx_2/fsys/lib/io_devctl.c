
/*
 *  io_devctl.c
 *
 *  Copyright 2002, 2003, 2012 QNX Software Systems Limited. All Rights Reserved.
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
 *  This module is the CAPTIVE FILESYSTEM utility library's io_devctl handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
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

// these are required for dcmd_blk.h
#include <sys/types.h>
#include <stdint.h>
#include <sys/disk.h>
#include <sys/fs_stats.h>
#include <sys/dcmd_blk.h>

#include "cfs.h"

int
cfs_io_devctl (resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    int     sts;
    void    *data;
    int     nbytes;
    int     dcmd;
    int     len;
    iov_t   iov [2];            // a two parter is what we use for our direct returns...

    // data types we can accept
    struct  fs_directio         *p_fs_directio;
    // data types we can return
    union {
        partition_entry_t       r_partition_entry;
        struct  fs_stats        r_fs_stats;
        struct  statvfs         r_statvfs;
        char                    r_buf [256];
    }   rval;

/*
 *  WARNING:  check in main.c to see if you need to adjust the "nparts_max" and "msg_max_size"
*/

D   printf ("%s %d:  cfs_io_devctl (dcmd 0x%08X)\n", __FILE__, __LINE__, msg -> i.dcmd);

    // see if it's a standard POSIX-supported devctl()
    if ((sts = iofunc_devctl_default (ctp, msg, ocb)) != _RESMGR_DEFAULT) {
D       printf ("%s %d:  cfs_io_devctl (dcmd 0x%08X) handled by iofunc_devctl_default, returns 0x%08X\n", __FILE__, __LINE__, msg -> i.dcmd, sts);
        return (sts);
    }

    // assign a pointer to the data area of the message
    data = _DEVCTL_DATA (*msg);

    // get other data that can't be wiped out before we wipe the data structure
    dcmd = msg -> i.dcmd;
    len = msg -> i.nbytes;
    memset (&msg -> o, 0, sizeof (msg -> o));

    // preset the number of bytes that we'll return to zero
    nbytes = 0;

    // set up the IOV
    SETIOV (&iov [0], &msg -> o, sizeof (msg -> o));
    SETIOV (&iov [1], &rval, sizeof (rval));
    memset (&rval, 0, sizeof (rval));

    sts = EOK;

    // check for all commands
    switch (dcmd) {
    case    DCMD_BLK_PARTENTRY:         // returns struct partition_entry
D       printf ("%s %d:  cfs_io_devctl DCMD_BLK_PARTENTRY\n", __FILE__, __LINE__);
        return (ENOSYS);

    case    DCMD_BLK_FORCE_RELEARN:     // no data
D       printf ("%s %d:  cfs_io_devctl DCMD_BLK_FORCE_RELEARN\n", __FILE__, __LINE__);
        return (ENOSYS);

    case    DCMD_FSYS_STATISTICS:       // returns struct fs_stats
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_STATISTICS\n", __FILE__, __LINE__);
        return (ENOSYS);

    case    DCMD_FSYS_STATISTICS_CLR:   // returns struct fs_stats
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_STATISTICS_CLR\n", __FILE__, __LINE__);
        return (ENOSYS);

    case    DCMD_FSYS_STATVFS:          // returns struct statvfs
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_STATVFS\n", __FILE__, __LINE__);
        cfs_block_fill_statvfs (ocb -> attr, &rval.r_statvfs);
        nbytes = sizeof (rval.r_statvfs);
        break;

    case    DCMD_FSYS_DIRECT_IO:        // accepts struct fs_directio
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_DIRECT_IO\n", __FILE__, __LINE__);
        p_fs_directio = data;
D       printf ("\toffset   %lld\n", p_fs_directio -> offset);
D       printf ("\tnbytes   %d\n", p_fs_directio -> nbytes);
D       printf ("\tflags    0x%08X\n", p_fs_directio -> flags);
D       // printf ("\taddress  0x%08X\n", p_fs_directio -> address);    // Kirk's machine doesn't have "address" member... version issue, no doubt
        return (ENOSYS);                // wouldn't know what to do with this one...
        break;

    case    DCMD_FSYS_MOUNTED_ON:       // returns char [256]
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_MOUNTED_ON\n", __FILE__, __LINE__);
        strcpy (data, "mounted_on");
        nbytes = strlen (data);
        break;

    case    DCMD_FSYS_MOUNTED_AT:       // returns char [256]
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_MOUNTED_AT\n", __FILE__, __LINE__);
        strcpy (data, "mounted_at");
        nbytes = strlen (data);
        break;

    case    DCMD_FSYS_MOUNTED_BY:       // returns char [256]
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_MOUNTED_BY\n", __FILE__, __LINE__);
        strcpy (rval.r_buf, "/ramdisk");
        nbytes = strlen (data);
        break;

    case    DCMD_FSYS_OPTIONS:          // returns char [256]
D       printf ("%s %d:  cfs_io_devctl DCMD_FSYS_OPTIONS\n", __FILE__, __LINE__);
        break;


    default:
D       printf ("%s %d:  cfs_io_devctl unknown, returning ENOSYS\n", __FILE__, __LINE__);
        return (ENOSYS);
    }

    // return data (if any) to the client
    msg -> o.nbytes = nbytes;
    MsgReplyv (ctp -> rcvid, sts, iov, 2);
    return (_RESMGR_NOREPLY);
}

