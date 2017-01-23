
/*
 *  c_mknod.c
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
 *  This module is the TAR Filesystem's c_mknod handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2002 12 21  R. Krten        change io_ to c_ for "connect"
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
cfs_c_mknod (resmgr_context_t *ctp, io_mknod_t *msg, RESMGR_HANDLE_T *handle, void *reserved)
{
    des_t   parent, target;
    int     sts;
    struct  _client_info cinfo;

D   printf ("%s %d:  cfs_c_mknod (%s)\n", __FILE__, __LINE__, msg -> connect.path);

    if (sts = iofunc_client_info (ctp, 0, &cinfo)) {
        return (sts);
    }

    if (connect_msg_to_attr (ctp, &msg -> connect, handle, &parent, &target, &sts, &cinfo)) {
        return (sts);
    }

    if (target.attr) {
D       printf ("%s %d:  cfs_c_mknod node exists\n", __FILE__, __LINE__);
        return (EEXIST);
    }

    if (sts = iofunc_mknod (ctp, msg, &target.attr -> attr, &parent.attr -> attr, &cinfo)) {
D       printf ("%s %d:  cfs_c_mknod called iofunc_mknod which returns %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
        return (sts);
    }

    if (!cfs_a_mkdir (parent.attr, target.name, &cinfo)) {                      // create new directory in parent
D       printf ("%s %d:  cfs_c_mknod unable to make target %d (%s)\n", __FILE__, __LINE__, errno, strerror (errno));
        return (errno);
    }

    return (EOK);
}

