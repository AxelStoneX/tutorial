
/*
 *  c_readlink.c
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
 *  This module is the RAMDISK's c_readlink handler.
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
cfs_c_readlink (resmgr_context_t *ctp, io_readlink_t *msg, RESMGR_HANDLE_T *handle, void *reserved)
{
    des_t   parent, target;
    int     sts;
    int     eflag;
    struct  _client_info cinfo;
    int     tmp;

D   printf ("%s %d:  cfs_c_readlink (%s), attr %p\n", __FILE__, __LINE__, msg -> connect.path, handle);

    if (sts = iofunc_client_info (ctp, 0, &cinfo)) {
        return (sts);
    }

    if (connect_msg_to_attr (ctp, &msg -> connect, handle, &parent, &target, &sts, &cinfo)) {
        return (sts);
    }
    if (!target.attr) {
        return (sts);
    }

    sts = iofunc_readlink (ctp, msg, &target.attr -> attr, NULL);
    if (sts != EOK) {
D       printf ("%s %d:  cfs_c_readlink called iofunc_readlink, and it returned %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
        return (sts);
    }

    // preserve eflag...
    eflag = msg -> connect.eflag;
    memset (&msg -> link_reply, 0, sizeof (msg -> link_reply));
    msg -> link_reply.eflag = eflag;

    tmp = strlen (target.attr -> type.symlinkdata);
    SETIOV (&ctp -> iov [0], &msg -> link_reply, sizeof (msg -> link_reply));
    SETIOV (&ctp -> iov [1], target.attr -> type.symlinkdata, tmp);
    msg -> link_reply.path_len = tmp;
    MsgReplyv (ctp -> rcvid, EOK, ctp -> iov, 2);
    return (_RESMGR_NOREPLY);
}

