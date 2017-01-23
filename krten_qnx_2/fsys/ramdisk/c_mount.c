
/*
 *  c_mount.c
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
 *  This module is the RAMDISK's c_mount handler.
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

#include <sys/dispatch.h>

#include "cfs.h"
#include <sys/iofunc.h>

#define OUR_MOUNT_STRING "ramdisk"

extern  resmgr_connect_funcs_t  connect_func;       // (main.c) connect funcs
extern  resmgr_io_funcs_t       io_func;            // (main.c) io funcs
extern  dispatch_t              *dpp;               // (main.c) dispatch structure
extern  resmgr_attr_t           resmgr_attr;        // (main.c) attributes for resource manager

int
cfs_c_mount (resmgr_context_t *ctp, io_mount_t *msg, RESMGR_HANDLE_T *handle, io_mount_extra_t *extra)
{
    char    *mnt_point;
    char    *mnt_type;
    char    *spec_name;
    char    *options;
    int     ret;
    cfs_attr_t  *cfs_attr;

D   printf ("%s %d:  cfs_c_mount (%s)\n", __FILE__, __LINE__, msg -> connect.path);

    // We need to have a length of the extra buffer to be able to read it in
    mnt_point = msg -> connect.path;
    mnt_type = extra -> extra.srv.type;
    spec_name = extra -> extra.srv.special;
    options = extra -> extra.srv.data;

D   printf("Mount information: \n");
D   printf("Mount point:        [%s]\n", mnt_point);
D   printf("Mount type:         [%s]\n", (mnt_type) ? mnt_type : "NULL");
D   printf("Special string:     [%s]\n", (spec_name) ? spec_name : "NULL");
D   printf("Options:            [%s]\n", (options) ? options : "NULL");
D   printf("Mount flags:        0x%x\n", extra->flags);
D   printf("Extra type          0x%x\n", msg -> connect.extra_type);

    // Verify that it is a mount request, not something else
    if (extra -> flags & (_MOUNT_ENUMERATE | _MOUNT_UNMOUNT | _MOUNT_REMOUNT)) {
D       printf ("%s %d:  cfs_c_mount request not supported yet\n", __FILE__, __LINE__);
        return (ENOTSUP);
    }

    /* 
     Decide if this request is for us or not.  In general we can 
     just check the type string, but if you want to support enumeration, 
     then you may or may not receive a type string and instead have to 
     iterate over something based on either the special device name or 
     an ocb.  In this manager we expect an ocb (though enumeration isn't supported).
    */
    if(!mnt_type || strcmp(mnt_type, OUR_MOUNT_STRING)) {
D       printf ("%s %d:  cfs_c_mount not our type (%s != %s) -> ENOSYS\n", __FILE__, __LINE__, mnt_type, OUR_MOUNT_STRING);
        return ENOSYS;
    }

    //NOTE: Other permission checking performed as required

    /*
     Initialize the new attribute for the device we are about
     to attach, we decide to inherit the server ownership

     NOTE: This example doesn't keep track of the attributes (we need to put them on a list, maybe?)
    */
    if (!(cfs_attr = malloc (sizeof (*cfs_attr)))) {
CP
        return ENOMEM;
    }
    iofunc_attr_init (&cfs_attr -> attr, S_IFDIR | 0777, NULL, NULL);
    cfs_attr_init (cfs_attr);                                           // initializes extended attribute structure
    cfs_attr -> attr.inode = (int) cfs_attr;                                // set up the inode
    cfs_a_mknod (cfs_attr, ".", S_IFDIR | 0755, NULL);
    cfs_a_mknod (cfs_attr, "..", S_IFDIR | 0755, NULL);

    //Attach the new pathname with the new value
D   printf ("%s %d:  creating new mountpoint (%s)\n", __FILE__, __LINE__, mnt_point);
    ret = resmgr_attach (dpp, &resmgr_attr, mnt_point, _FTYPE_ANY, _RESMGR_FLAG_DIR, &connect_func, &io_func, &cfs_attr -> attr);   
    if (ret == -1) {
        free (cfs_attr);
CP
        return (errno);
    }
CP
    return (EOK);
}

