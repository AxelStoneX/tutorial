
/*
 *  c_rename.c
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
 *  This module is the RAMDISK's c_rename handler.
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
 *  cfs_c_rename
 *
 *  This implements the rename() functionality.
 *  We assume that the source name is on our device and has no symlinks.
 *
 *  Since we are dealing with two paths, we need to call the lower-level pathwalk()
 *  function on the source pathname to get at the attribute structures there, but we
 *  can still use the high-level connect_msg_to_attr() function on the target -- after
 *  all, the target might need to be redirected, etc.
*/

int
cfs_c_rename (resmgr_context_t *ctp, io_rename_t *msg, RESMGR_HANDLE_T *handle, io_rename_extra_t *extra)
{
    des_t   sparent, starget, dparent, dtarget;                                 // source and destination parents and targets
    des_t   components [_POSIX_PATH_MAX];
    int     ncomponents;
    int     sts;
    char    *p;
    int     i;
    struct  _client_info cinfo;

D   printf ("%s %d:  cfs_c_rename (%s -> %s)\n", __FILE__, __LINE__, extra -> path, msg -> connect.path);

    /*
     *  In order to prevent "mv x x/a" from working, we need to verify
     *  that the source is *not* an initial substring of (nor identical to) the
     *  destination.  We compare the source and destination for the length
     *  of the source.  If the comparison is identical, we check to see
     *  what the character in the destination *after* the comparison
     *  was (it's legitimate to do "mv x xa", with "x" being a substring
     *  of "xa").  If there is no character after the comparison match, it
     *  means that the strings are identical.  If there is a character, it
     *  better not be a forward slash, 'cuz that would imply that the source
     *  will be moved into a subordinate directory (and therefore lost).
    */

    i = strlen (extra -> path);
    if (!strncmp (extra -> path, msg -> connect.path, i)) {                     // source could be a subset, check character after end of subset in destination
        if (msg -> connect.path [i] == 0 || msg -> connect.path [i] == '/') {   // source is identical to destination, or source is a subset
            return (EINVAL);
        }
    }

    /*
     *  Handle the destination resolution first in case we need to do a redirect
     *  or otherwise fail the request.
    */

    if (sts = iofunc_client_info (ctp, 0, &cinfo)) {
        return (sts);
    }

    if (connect_msg_to_attr (ctp, &msg -> connect, handle, &dparent, &dtarget, &sts, &cinfo)) {
        return (sts);
    }

    // see if there are any problems (destination shouldn't exist)
    // kill it if it does exist
    if (sts != ENOENT) {
        if (sts == EOK) {
            if ((sts = cfs_rmnod (&dparent, dtarget.name, dtarget.attr)) != EOK) {
                return (sts);
            }
        } else {
            return (sts);
        }
    }

    /*
     *  The source resolution is done using the pathwalk() function -- we are
     *  guaranteed that the source is on our filesystem and that there are no
     *  symlinks, so we just need to walk it to get the attribute structures.
    */

D   printf ("%s %d:  cfs_c_rename resolution of destination ok, proceeding to source\n", __FILE__, __LINE__);
    ncomponents = _POSIX_PATH_MAX;
    sts = pathwalk (ctp, extra -> path, handle, 0, components, &ncomponents, &cinfo);
D   dump_pw (sts, ncomponents, components);

    // missing directory component
    if (sts == ENOTDIR) {
D       printf ("%s %d:  cfs_c_rename, source is missing directories\n", __FILE__, __LINE__);
        return (sts);
    }

    // missing non-final component
    if (components [ncomponents].name != NULL && sts == ENOENT) {
D       printf ("%s %d:  cfs_c_rename, source is missing a non-final component, returns %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
        return (sts);
    }

    if (ncomponents < 2) {
        return (EBUSY);     // can't move the root directory of the filesystem
    }

    starget = components [ncomponents - 1];
    sparent = components [ncomponents - 2];

    /*
     *  Link it into the new location first, and then unlink it from the old
     *  location, maintaining counts for each operation in progress.
    */

    p = strdup (dtarget.name);
    if (p == NULL) {
        return (ENOMEM);
    }

    if (sts = add_new_dirent (dparent.attr, starget.attr, p)) {
        free (p);
        return (sts);
    }
    starget.attr -> attr.nlink++;

    return (cfs_rmnod (&sparent, starget.name, starget.attr));
}

