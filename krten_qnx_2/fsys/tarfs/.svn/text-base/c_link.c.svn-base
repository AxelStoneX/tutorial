
/*
 *  c_link.c
 *
 *  Copyright 2003 QNX Software Systems Limited. All Rights Reserved.
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
 *  This module is the TAR Filesystem's c_link handler.
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
cfs_c_link (resmgr_context_t *ctp, io_link_t *msg, RESMGR_HANDLE_T *handle, io_link_extra_t *extra)
{
    RESMGR_OCB_T    *ocb;
    des_t           parent, target;
    int             sts;
    char            *p, *s;
    struct          _client_info cinfo;

D   printf ("%s %d:  cfs_c_link (%s)\n", __FILE__, __LINE__, msg -> connect.path);
D   dump_c_connect (&msg -> connect);

    if (sts = iofunc_client_info (ctp, 0, &cinfo)) {
        return (sts);
    }

    if (connect_msg_to_attr (ctp, &msg -> connect, handle, &parent, &target, &sts, &cinfo)) {
        return (sts);
    }
    if (target.attr) {
        return (EEXIST);
    }

    switch (msg -> connect.extra_type) {
    case    _IO_CONNECT_EXTRA_LINK:         // hardlink (add msg -> connect.path to a directory; point attribute to ocb -> attr; bump link count)
        ocb = extra -> ocb;
D       printf ("%s %d:  cfs_c_link () doing a hardlink to inode %llX\n", __FILE__, __LINE__, ocb -> attr -> attr.inode);
        p = strdup (target.name);
        if (p == NULL) {
            return (ENOMEM);
        }
        if (sts = add_new_dirent (parent.attr, ocb -> attr, p)) {
            free (p);
            return (sts);
        }
        ocb -> attr -> attr.nlink++;
        return (EOK);

    case    _IO_CONNECT_EXTRA_SYMLINK:      // symlink
D       printf ("%s %d:  cfs_c_link () doing a symlink to \"%s\"\n", __FILE__, __LINE__, extra -> path);
        p = target.name;
        s = strdup (extra -> path);
        if (s == NULL) {
            free (s);
            return (ENOMEM);
        }
        target.attr = cfs_a_mksymlink (parent.attr, p, NULL);
        if (!target.attr) {
            free (s);
            return (errno);
        }
        target.attr -> type.symlinkdata = s;
        target.attr -> attr.nbytes = strlen (s); 
        target.attr -> attr.rdev = 1234;
        return (EOK);

    default:
D       printf ("%s %d:  cfs_c_link () unknown extra type 0x%02X\n", __FILE__, __LINE__, msg -> connect.extra_type);
        return (ENOSYS);
    }

    return (_RESMGR_DEFAULT);
}

