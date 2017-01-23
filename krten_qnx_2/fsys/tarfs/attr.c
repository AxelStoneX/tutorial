
/*
 *  attr.c
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
 *  This module is the TAR Filesystem's attribute handling module.
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

/*
 *  release_attr
 *
 *  This function releases an extended attributes structure after checking
 *  to see that it's ok to do so:
 *
 *  o   the usage count must be zero, otherwise it's in use
 *  o   if it's a directory, it has to be empty (just "." and "..")
 *  o   if it's a file, it has to have only one link
 *  o   if it's a symlink, just go ahead and delete it
 *
 *  It's important to examine the return code and discontinue further operations;
 *  for example, if we're trying to release a non-empty directory, you can't
 *  continue the higher-level function (in io_unlink, for example) of releasing
 *  the parent's entry...
*/

int
release_attr (cfs_attr_t *attr)
{

D   printf ("%s %d:  release_attr (%p, nlink %d, count %d)\n", __FILE__, __LINE__, attr, attr -> attr.nlink, attr -> attr.count);

    if (!attr -> attr.nlink  && !attr -> attr.count) {                          // count must be zero in order to delete it; otherwise someone else is still using it!
        // decide what kind (file or dir) this entry is...

        if (S_ISDIR (attr -> attr.mode)) {                                      // it's a directory
            if (attr -> nels > 2) {                                             // and it's not empty!
                return (ENOTEMPTY);
            }
            // need to free "." and ".."
            free (attr -> type.dirblocks [0].name);
            free (attr -> type.dirblocks [0].attr);
            free (attr -> type.dirblocks [1].name);
            free (attr -> type.dirblocks [1].attr);

            if (attr -> type.dirblocks) {
                free (attr -> type.dirblocks);
                attr -> type.dirblocks = NULL;
                free (attr);
            } else CP
        } else if (S_ISREG (attr -> attr.mode)) {                               // it's a file
            // we don't delete in this filesystem :-)
        } else if (S_ISLNK (attr -> attr.mode)) {                               // it's a symlink
            free (attr -> type.symlinkdata);
            free (attr);
        } else {
CP D        printf ("%s %d:  unexpected mode 0x%x (0%o)!\n", __FILE__, __LINE__, attr -> attr.mode, attr -> attr.mode);
        }
    }
    return (EOK);
}

