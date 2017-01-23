
/*
 *  io_read.c
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

int
cfs_io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int     sts;

    // use the helper function to decide if valid
    if ((sts = iofunc_read_verify (ctp, msg, ocb, NULL)) != EOK) {
        return (sts);
    }

    // decide if we should perform the "file" or "dir" read
    if (S_ISDIR (ocb -> attr -> attr.mode)) {
        return (ramdisk_io_read_dir (ctp, msg, ocb));
    } else if (S_ISREG (ocb -> attr -> attr.mode)) {
        return (ramdisk_io_read_file (ctp, msg, ocb));
    } else {
CP
        return (EBADF);
    }
}

