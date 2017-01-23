
/*
 *  c_unlink.c
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
 *  This module is the RAMDISK's c_unlink handler.
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

/*
 *  cfs_c_unlink
 *
 *  This is the unlink() handler.
*/

int
cfs_c_unlink (resmgr_context_t *ctp, io_unlink_t *msg, RESMGR_HANDLE_T *handle, void *reserved)
{
    des_t   parent, target;
    int     sts;
    struct  _client_info cinfo;

D   printf ("%s %d:  cfs_c_unlink (%s)\n", __FILE__, __LINE__, msg -> connect.path);
    
    if (sts = iofunc_client_info (ctp, 0, &cinfo)) {
        return (sts);
    }

    if (connect_msg_to_attr (ctp, &msg -> connect, handle, &parent, &target, &sts, &cinfo)) {
        return (sts);
    }

    /*
     *  If the status is anything other than EOK, we abort.  The thing
     *  that we're unlinking *must* exist.  Also, it must not be the mountpoint
     *  (in which case we return EBUSY)
    */

    if (sts != EOK) {
        return (sts);
    }

    if (target.attr == handle) {
D       printf ("%s %d:  cfs_c_unlink attempts to remove the mountpoint, returning EBUSY\n", __FILE__, __LINE__);
        return (EBUSY);
    }

    return (cfs_rmnod (&parent, target.name, target.attr));
}

int
cfs_rmnod (des_t *parent, char *name, cfs_attr_t *attr)
{
    int     sts;
    int     i;

    /*
     *  FIRST, we remove the target via release_attr
    */

    attr -> attr.nlink--;
    if ((sts = release_attr (attr)) != EOK) {
D       printf ("%s %d:  cfs_rmnod called release_attr which returned %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
        return (sts);
    }

    /*
     *  SECOND, we remove the directory entry out of the parent
    */

    for (i = 0; i < parent -> attr -> nels; i++) {
        if (parent -> attr -> type.dirblocks [i].name == NULL) {
            continue;
        }
        if (!strcmp (parent -> attr -> type.dirblocks [i].name, name)) {
            break;
        }
    }
    if (i == parent -> attr -> nels) {
D       printf ("%s %d:  cfs_rmnod (\"%s\") not found; ENOENT\n", __FILE__, __LINE__, name);
        return (ENOENT);                                                        // huh.  gone.  This is either some kind of internal error, or a race condition.
    }

D   printf ("%s %d:  cfs_rmnod (\"%s\") freeing the directory entry at %p\n", __FILE__, __LINE__, name, parent -> attr -> type.dirblocks [i].attr);
    free (parent -> attr -> type.dirblocks [i].name);
    parent -> attr -> type.dirblocks [i].name = NULL;

    while (parent -> attr -> type.dirblocks [parent -> attr -> nels - 1].name == NULL) {    // this will be stopped by ".." which always exists
        parent -> attr -> nels--;                                                       // shrink directories only if it's safe to do so
    }
    // could check the open count and do other reclamation magic here, but we don't *have to* for now...

    return (EOK);
}

