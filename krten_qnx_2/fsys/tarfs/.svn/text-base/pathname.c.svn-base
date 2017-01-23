
/*
 *  pathname.c
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
 *  This module is the TAR Filesystem's pathname handling
 *  functions.
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

#if 0   // we don't use blocks in this fsys
static int add_blocks (cfs_attr_t *attr, int sblk, int nblks, int flags);
static void wipe_block_to_end (cfs_attr_t *attr, int blk, int offset);
static void remove_blocks (cfs_attr_t *attr, int sblk, int nblks, int flags);
#endif

/*
 *  cfs_a_mkfile
 *  cfs_a_mkdir
 *
 *  Entry point to create a file or directory based on an attribute.  The attribute that's
 *  passed is the "current directory" in which the entity is created.  The
 *  null parameter defines the characteristics of the entity (currently not used).
*/

cfs_attr_t *
cfs_a_mknod (cfs_attr_t *attr, char *fname, int mode, struct _client_info *cinfo)
{
    cfs_attr_t  *newattr;

D   if (cinfo) printf ("%s %d:  cfs_a_mknod (attr %p, fname \"%s\", mode 0x%x (0%o), cinfo [nd %d pid %d  sid %d flags %d cred -> [ruid %d euid %d suid %d rgid %d egid %d sgid %d ngroups %d]]\n", __FILE__, __LINE__,
            attr, fname, mode, mode, cinfo -> nd, cinfo -> pid, cinfo -> sid, cinfo -> flags,
            cinfo -> cred.ruid, cinfo -> cred.euid, cinfo -> cred.suid, cinfo -> cred.rgid, cinfo -> cred.egid, cinfo -> cred.sgid, cinfo -> cred.ngroups);
D   if (!cinfo) printf ("%s %d:  cfs_a_mknod (attr %p, fname \"%s\", mode 0x%x (0%o), cinfo NULL\n", __FILE__, __LINE__,  attr, fname, mode, mode);
        
    newattr = add_new_node_and_dirent (attr, fname);
    if (!newattr) {
        return (NULL);
    }

    cfs_attr_init (newattr);
    iofunc_attr_init (&newattr -> attr, mode, &attr -> attr, 0);
    newattr -> attr.inode = (int) newattr;
    newattr -> attr.atime = newattr -> attr.mtime = newattr -> attr.ctime = time (NULL);    // they are all created equal...
    newattr -> attr.rdev = 7;
    if (cinfo) {
        newattr -> attr.uid = cinfo -> cred.euid;
        newattr -> attr.gid = cinfo -> cred.egid;
    } // else defaults to root:root

    return (newattr);
}

cfs_attr_t *
cfs_a_mkfile (cfs_attr_t *attr, char *fname, struct _client_info *cinfo)
{
    return (cfs_a_mknod (attr, fname, S_IFREG | 0644, cinfo));
}

cfs_attr_t *
cfs_a_mkdir (cfs_attr_t *attr, char *fname, struct _client_info *cinfo)
{
    cfs_attr_t  *new;

    new = cfs_a_mknod (attr, fname, S_IFDIR | 0755, cinfo);
    if (new) {
        cfs_a_mknod (new, ".", S_IFDIR | 0755, cinfo);
        cfs_a_mknod (new, "..", S_IFDIR | 0755, NULL);                          // ".." should *not* get anything from this client...
    }
    return (new);
}

cfs_attr_t *
cfs_a_mksymlink (cfs_attr_t *attr, char *fname, void *null)
{
    return (cfs_a_mknod (attr, fname, S_IFLNK | 0777, NULL));
}

void
cfs_a_rmattr (cfs_attr_t *parent, cfs_attr_t *target)
{
D   printf ("%s %d cfs_a_rmattr parent %p target %p *** UNIMPLEMENTED ***\n", __FILE__, __LINE__, parent, target);
}

cfs_attr_t *
search_dir (char *component, cfs_attr_t *parent)
{
    int     i;

    for (i = 0; i < parent -> nels; i++) {

        if (parent -> type.dirblocks [i].name == NULL) {                        // skip empty ones
            continue;
        }
        if (!strcmp (parent -> type.dirblocks [i].name, component)) {           // found it!
D           printf ("%s %d:  search_dir (\"%s\" in parent %p) element [%d] (%s) MATCHED (%p)\n", __FILE__, __LINE__, component, parent, i, parent -> type.dirblocks [i].name, parent -> type.dirblocks [i].attr);

            return (parent -> type.dirblocks [i].attr);
        }
    }

    return (NULL);
}

/*
 *  pathwalk
 *
 *  This function performs two steps:
 *      a) the slash-separated pathname is broken into individual pathname
 *         components, and the ".name" members of the output structure array
 *         are filled with the components.
 *      b) checking is performed, starting at the root, to ensure accessability
 *         to each component.  Processing aborts when a component is not
 *         found, is not a directory and needs to be, the client doesn't have
 *         permissions to access the directory, or the entry is a symlink.
 *
 *  Note that *nrets is passed in as the number of elements in the output
 *  array, and E2BIG is returned if there are more elements present.
 *
 *  ctp and cinfo are optional.  If either or both are NULL, then the
 *  iofunc_check_access() test is skipped.  This is useful for testing
 *  only against pathname accessibility.
 *
 *  The return values from pathwalk are as follows:
 *
 *  -   "output" is filled with the pathnames and attribute structures of
 *      each pathname component.  The first element (output [0]) corresponds
 *      to the mountpoint, and as such, has a ".name" member of the empty
 *      string ("").
 *      Elements past the "*nrets" return value may have valid pathnames,
 *      as this is done in the first step to make it simple to detect if
 *      components follow a particular component.
 *  -   "nrets" contains the number of elements successfully processed.
 *      It will always be at least 1, because there is always at least
 *      the (implied) mountpoint.
*/

int
pathwalk (resmgr_context_t *ctp, char *pathname, cfs_attr_t *mountpoint, int flags, des_t *output, int *nrets, struct _client_info *cinfo)
{
    int                     nels;
    int                     sts;
    char                    *p;

D   printf ("%s %d:  pathwalk (\"%s\") at mountpoint %p with flags 0x%08x\n", __FILE__, __LINE__, pathname, mountpoint, flags);
D   printf ("%s %d:  pathwalk breaking %s into ", __FILE__, __LINE__, pathname); D fflush (stdout);

    // first, we break apart the slash-separated pathname
    memset (output, 0, sizeof (output [0]) * *nrets);
    output [0].attr = mountpoint;                                               // a given; we always start at the mountpoint
    output [0].name = "";                                                       // and the mountpoint is always the empty string

    for (nels = 1, p = strtok (pathname, "/"); p; p = strtok (NULL, "/"), nels++) {
        if (nels >= *nrets) {
            return (E2BIG);
        }
        output [nels].name = p;
D       printf ("%s ", p); D fflush (stdout);
        output [nels].attr = NULL;
    }
D   printf ("\n");

    // next, we analyze each pathname
    for (*nrets = 1; *nrets < nels; ++*nrets) {

D       printf ("%s %d:  pathwalk analyzing component \"%s\" (*nrets %d)\n", __FILE__, __LINE__, output [*nrets].name, *nrets);

        /*
         *  in order to be able to process another pathname component ("output [*nrets].name" in
         *  this case), the previous pathname component (*nrets - 1) must be
         *  a directory, and ...
        */

        if (!S_ISDIR (output [*nrets - 1].attr -> attr.mode)) {
D           printf ("%s %d:  pathwalk previous component %d (%s) -> ENOTDIR, therefore can't process this component (%s)\n", __FILE__, __LINE__, *nrets - 1, output [*nrets - 1].name, output [*nrets].name);
            return (ENOTDIR);       // *nrets includes the non-directory
        }

        /*
         *  ... we must have access permissions.
        */

        if (ctp && cinfo) {                                                     // both must exist; otherwise we're not checking accessability, just path correctness
            if ((sts = iofunc_check_access (ctp, &output [*nrets - 1].attr -> attr, S_IEXEC, cinfo)) != EOK) {
D               printf ("%s %d:  pathwalk component %d (%s) check_access returns %d (%s)\n", __FILE__, __LINE__, *nrets - 1, output [*nrets - 1].name, sts, strerror (sts));
                return (sts);           // *nrets includes the non-accessible directory
            }
        }

        /*
         *  At this point we have the ability to do a directory seek, so we are
         *  actually working on behalf of the entry "output [*nrets].name"
         *  Check to see if the directory exists, if so...
        */

        if (!(output [*nrets].attr = search_dir (output [*nrets].name, output [*nrets - 1].attr))) {
D           printf ("%s %d:  pathwalk component %d (%s) within component %d (%s) -> ENOENT\n", __FILE__, __LINE__, *nrets, output [*nrets].name, *nrets - 1, output [*nrets - 1].name);
            ++*nrets;
            return (ENOENT);        // *nrets includes the entry
        }

D       printf ("%s %d:  pathwalk matched component %d (%s) -> %p\n", __FILE__, __LINE__, *nrets, output [*nrets].name, output [*nrets].attr);

        /*
         *  ... process the entry itself.
         *  We check to see if it's a symbolic link, and abort if it is.
         *  (Symbolic links are handled higher up).
        */

        if (S_ISLNK (output [*nrets].attr -> attr.mode)) {
D           printf ("%s %d:  pathwalk (\"%s\", component %d) -> S_ISLNK\n", __FILE__, __LINE__, pathname, *nrets);
            ++*nrets;
            return (EOK);           // *nrets includes the entry
        }
    }

    return (EOK);
}

/*
 *  dump_pw
 *
 *  A utility/debug function to dump the pathwalk output.
*/

void
dump_pw (int sts, int ncomponents, des_t *components)
{
    int     j;

    printf ("--pathwalk component dump: ncomponents %d, returns %d (%s)\n", ncomponents, sts, strerror (sts));
    for (j = 0; j < ncomponents; j++) {
        printf ("  [%d] attr %p name \"%s\"\n", j, components [j].attr, components [j].name);
    }
}
