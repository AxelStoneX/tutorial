
/*
 *  io_close.c
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
 *  This module is the CAPTIVE FILESYSTEM utility library's io_close handler.
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

extern int optd;

int
cfs_io_close_ocb (resmgr_context_t *ctp, void *reserved, RESMGR_OCB_T *ocb)
{
    cfs_attr_t      *attr;
    int         sts;

D   printf ("%s %d:  cfs_io_close_ocb (ocb %p attr %p)\n", __FILE__, __LINE__, ocb, ocb -> attr);

    attr = ocb -> attr;
    sts = iofunc_close_ocb (ctp, ocb, &attr -> attr);
    if (sts == EOK) {
        sts = release_attr (attr);                                              // release_attr makes sure that no-one is using it...
    }
    return (sts);
}

int
cfs_io_close_dup (resmgr_context_t *ctp, io_close_t *msg, RESMGR_OCB_T *ocb)
{
D   printf ("%s %d:  cfs_io_close_dup ()\n", __FILE__, __LINE__);
    return (iofunc_close_dup_default (ctp, msg, ocb));
}

