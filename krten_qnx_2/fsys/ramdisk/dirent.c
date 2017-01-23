
/*
 *  dirent.c
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
 *  This module is the RAMDISK's directory entry
 *  handling functions.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated.
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

#define ALIGN(x) (((x) + 3) & ~3)

extern int optd;

#define NBLOCKS_ALLOCATED_AT_ONCE       64      // 64 x 8 = 512

cfs_attr_t *
add_new_node_and_dirent (cfs_attr_t *attr, char *fname)
{
    cfs_attr_t  *newattr;
    char        *p;

D   printf ("%s %d:  add_new_node_and_dirent (attr %p fname %s)\n", __FILE__, __LINE__, attr, fname);

    // perform all our allocations (and possible releases) up front
    newattr = malloc (sizeof (*newattr));
    p = strdup (fname);
    if (!newattr || !p) {
        if (newattr) {
            free (newattr);
        }
        if (p) {
            free (p);
        }
D   printf ("%s %d:  add_new_node_and_dirent (attr %p fname %s) -> out of memory\n", __FILE__, __LINE__, attr, fname);
        return (NULL);
    }

    if (add_new_dirent (attr, newattr, p) != EOK) {
        // (need to think about error propagation here)
        free (p);
        free (newattr);
        return (NULL);
    }

D   printf ("%s %d:  add_new_node_and_dirent (attr %p fname %s) -> %p\n", __FILE__, __LINE__, attr, fname, newattr);

    return (newattr);
}

int
add_new_dirent (cfs_attr_t *parent, cfs_attr_t *newattr, char *newname)
{
    des_t       *newdirent;
    int         i;

D   printf ("%s %d:  add_new_dirent (parent %p newattr %p newname %s)\n", __FILE__, __LINE__, parent, newattr, newname);

    // scan for empty one
    for (i = 0; i < parent -> nels; i++) {
        if (parent -> type.dirblocks [i].name == NULL) {
            break;
        }
    }

    if (i == parent -> nels) {                                                  // couldn't find any, see if we need to make some
        if (parent -> nalloc <= parent -> nels) {                               // we need to allocate more
            newdirent = realloc (parent -> type.dirblocks, sizeof (des_t) * (parent -> nels + NBLOCKS_ALLOCATED_AT_ONCE));
            if (newdirent == NULL) {
D               printf ("%s %d:  add_new_dirent (parent %p newattr %p newname %s) realloc (%d bytes) failed\n", __FILE__, __LINE__, parent, newattr, newname, sizeof (des_t) * (parent -> nels + NBLOCKS_ALLOCATED_AT_ONCE));
                return (errno);
            }
            parent -> type.dirblocks = newdirent;
            parent -> nalloc += NBLOCKS_ALLOCATED_AT_ONCE;                      // just allocated more, better track it
        }
        i = parent -> nels;                                                     // set up "new" location
        parent -> nels++;                                                       // increment "nels" only if we actually added one
    }

    parent -> type.dirblocks [i].name = newname;
    parent -> type.dirblocks [i].attr = newattr;
    return (EOK);
}

int
dirent_size (char *fname)
{
    return (ALIGN (sizeof (struct dirent) + strlen (fname)));
}

struct dirent *
dirent_fill (struct dirent *dp, int inode, int offset, char *fname)
{
    dp -> d_ino = inode;
    dp -> d_offset = offset;
    strcpy (dp -> d_name, fname);
    dp -> d_namelen = strlen (dp -> d_name);
    dp -> d_reclen = ALIGN (sizeof (struct dirent) + dp -> d_namelen);
    return ((struct dirent *) ((char *) dp + dp -> d_reclen));
}

