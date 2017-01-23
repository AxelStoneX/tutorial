
/*
 *  cfs_func_init.c
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
 *  This module is the CAPTIVE FILESYSTEM utility library's cfs_func_init function.
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

void
cfs_func_init (resmgr_connect_funcs_t *cfuncs, resmgr_io_funcs_t *ifuncs)
{
    // for now, just blast over their functions; eventually, we'll want to expand this function and extend the attribute structure to store their original functions so we can call them
    // also, we'll want to check "nconnects" and "nios" to verify that we aren't going past the end of their structure.

    cfuncs -> open  = cfs_c_open;
    cfuncs -> unlink = cfs_c_unlink;
    cfuncs -> rename = cfs_c_rename;
    cfuncs -> mknod = cfs_c_mknod;
    cfuncs -> readlink = cfs_c_readlink;
    cfuncs -> link = cfs_c_link;
    cfuncs -> mount = cfs_c_mount;
    // we do not touch unblock

    ifuncs -> read  = cfs_io_read;
    ifuncs -> write = cfs_io_write;
    ifuncs -> close_ocb = cfs_io_close_ocb;
    ifuncs -> devctl = cfs_io_devctl;
    ifuncs -> space = cfs_io_space;
    // we do not touch stat notify unblock pathconf lseek chmod chown utime openfd fdinfo lock shutdown mmap msg dup close_dup lock_ocb unlock_ocb sync
}

