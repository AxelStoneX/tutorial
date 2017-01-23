
/*
 *  tarfs.c
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
 *  This module contains the tarfs support routines.
 *
 *  2003 01 12  R. Krten        created
 *  2003 01 15  R. Krten        added zlib support
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <tar.h>
#include <zlib.h>

#include "cfs.h"

#define TAR_NAMELEN                         99
#define TAR_MODELEN                         7
#define TAR_UIDLEN                          7
#define TAR_GIDLEN                          7
#define TAR_SIZELEN                         11
#define TAR_MTIMELEN                        11
#define TAR_CKSUMLEN                        7
#define TAR_LINKNAMELEN                     99
#define TAR_MAGICLEN                        5
#define TAR_VERSIONLEN                      2
#define TAR_UNAMELEN                        31
#define TAR_GNAMELEN                        31
#define TAR_DEVMAJORLEN                     7
#define TAR_DEVMINORLEN                     7
#define TAR_PREFIXLEN                       154

typedef struct ustar_s
{
                                            //  ------------------------------------
                                            //  Field Name     Offset       Length
                                            //  ------------------------------------
    char    name [TAR_NAMELEN + 1];         //  name              0          100
    char    mode [TAR_MODELEN + 1];         //  mode            100            8
    char    uid [TAR_UIDLEN + 1];           //  uid             108            8
    char    gid [TAR_GIDLEN + 1];           //  gid             116            8
    char    size [TAR_SIZELEN + 1];         //  size            124           12
    char    mtime [TAR_MTIMELEN + 1];       //  mtime           136           12
    char    cksum [TAR_CKSUMLEN + 1];       //  chksum          148            8
    char    typeflag;                       //  typeflag        156            1
    char    linkname [TAR_LINKNAMELEN + 1]; //  linkname        157          100
    char    magic [TAR_MAGICLEN + 1];       //  magic           257            6
    char    version [TAR_VERSIONLEN];       //  version         263            2
    char    uname [TAR_UNAMELEN + 1];       //  uname           265           32
    char    gname [TAR_GNAMELEN + 1];       //  gname           297           32
    char    devmajor [TAR_DEVMAJORLEN + 1]; //  devmajor        329            8
    char    devminor [TAR_DEVMINORLEN + 1]; //  devminor        337            8
    char    prefix [TAR_PREFIXLEN + 1];     //  prefix          345          155
                                            //  ------------------------------------
    char    filler [12];                    //  filler to round it out to 512 bytes
} ustar_t;

#define octal(n) strtol(n,NULL,8)

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

static  void    dump_tar_header (off_t off, ustar_t *t);
static  int     add_tar_entry (cfs_attr_t *a, off_t off, ustar_t *t, char *fname);
static  void    tar_to_attr (ustar_t *t, iofunc_attr_t *a);
static  int     parse_mode (ustar_t *t);

int
analyze_tar_file (cfs_attr_t *a, char *fname)
{
    gzFile  fd;
    off_t   off;
    ustar_t t;
    int     size;
    int     sts;
    char    *f;

    if ((fd = gzopen (fname, "r")) == NULL) {
D       printf ("%s %d:  analyze_tar_file, \"%s\" doesn't exist, errno %d (%s)\n", __FILE__, __LINE__, fname, errno, strerror (errno));
        return (errno);
    }

    off = 0;
    f = strdup (fname);     /// strdup to save strdup's in each individual type.vfile -> name in add_tar_entry; (enhance this by having the name in an "extended mountpoint structure")
    while (gzread (fd, &t, sizeof (t)) > 0 && *t.name) {
        dump_tar_header (off, &t);
        sscanf (t.size, "%o", &size);
        off += sizeof (t);
        if (sts = add_tar_entry (a, off, &t, f)) {
            gzclose (fd);
            return (sts);
        }
        off += ((size + 511) / 512) * 512;
        gzseek (fd, off, SEEK_SET);
    }
    gzclose (fd);

    return (EOK);
}

// copied from pathwalk()

static int
add_tar_entry (cfs_attr_t *a, off_t off, ustar_t *t, char *tarfile)
{
    des_t   output [_POSIX_PATH_MAX];
    int     nels;
    char    *p;
    int     i;
    int     mode;

    output [0].attr = a;
    output [0].name = NULL;

D   printf ("%s %d:  add_tar_entry (%s) [tarfile %s] -> ", __FILE__, __LINE__, t -> name, tarfile);
    for (nels = 1, p = strtok (t -> name, "/"); p; p = strtok (NULL, "/"), nels++) {
        if (nels >= _POSIX_PATH_MAX) {
D           printf ("\n%s %d:  add_tar_entry too long!\n", __FILE__, __LINE__);
            return (E2BIG);
        }
        output [nels].name = p;
D       printf ("%s ", p); D fflush (stdout);
        output [nels].attr = NULL;
    }
D   printf ("\n");

    // next, we analyze each pathname
    for (i = 1; i < nels; i++) {

D       printf ("%s %d:  add_tar_entry analyzing component \"%s\" (i %d)\n", __FILE__, __LINE__, output [i].name, i);

        /*
         *  in order to be able to process another pathname component ("output [i].name" in
         *  this case), the previous pathname component (i - 1) must be
         *  a directory.
        */

        if (!S_ISDIR (output [i - 1].attr -> attr.mode)) {
D           printf ("%s %d:  add_tar_entry previous component %d (%s) -> ENOTDIR, therefore can't process this component (%s)\n", __FILE__, __LINE__, i - 1, output [i - 1].name, output [i].name);
            return (ENOTDIR);       // effectively an internal error
        }

        // deleted access permissions checks here...

        /*
         *  At this point we have the ability to do a directory seek, so we are
         *  actually working on behalf of the entry "output [i].name"
         *  Check to see if the directory exists, if so...
        */

        if (!(output [i].attr = search_dir (output [i].name, output [i - 1].attr))) {
            mode = parse_mode (t);
            if (S_ISDIR (mode) || (i + 1 < nels)) { // has to be a directory, or an implied directory
D               printf ("%s %d:  add_tar_entry detected a directory (%s) %s\n", __FILE__, __LINE__, output [i].name, S_ISDIR (mode) ? "explicitly" : "implicitly");
                output [i].attr = cfs_a_mkdir (output [i - 1].attr, output [i].name, NULL);
                tar_to_attr (t, &output [i].attr -> attr);  // convert funky octal to binary data
                // kludge for implied "directories"
                if (S_ISREG (output [i].attr -> attr.mode)) {
                    output [i].attr -> attr.mode = (output [i].attr -> attr.mode & ~S_IFREG) | S_IFDIR;
                }
            } else if (S_ISREG (mode)) {
                output [i].attr = cfs_a_mkfile (output [i - 1].attr, output [i].name, NULL);
                tar_to_attr (t, &output [i].attr -> attr);  // convert funky octal to binary data
                output [i].attr -> nels = output [i].attr -> nalloc = 1;
                output [i].attr -> type.vfile.name = tarfile;                   // strdup'd above in analyze_tar_file, see comment
                output [i].attr -> type.vfile.off = off;
            } else if (S_ISLNK (mode)) {
                output [i].attr = cfs_a_mksymlink (output [i - 1].attr, output [i].name, NULL);
                tar_to_attr (t, &output [i].attr -> attr);  // convert funky octal to binary data
                output [i].attr -> type.symlinkdata = strdup (t -> linkname);
                output [i].attr -> attr.nbytes = strlen (t -> linkname);
            } else {
                printf ("%s %d:  add_tar_entry unknown mode for component %s 0x%x (0%o)\n", __FILE__, __LINE__, output [i].name, mode, mode);
                return (EBADF);         // effectively an internal error
            }

        }

D       printf ("%s %d:  add_tar_entry matched component %d (%s) -> %p\n", __FILE__, __LINE__, i, output [i].name, output [i].attr);

        /*
         *  ... process the entry itself.
         *  symlink checks deleted here
        */

    }

    return (EOK);
}

static void
tar_to_attr (ustar_t *t, iofunc_attr_t *a)
{
// QNX 6.1 tar has an incorrect signature (magic&version) but the fields are nul terminated, whereas QNX 6.1 pax has a good signature but the fields aren't nul terminated.  Sigh.
    t -> mode [TAR_MODELEN] = 0;
    t -> uid [TAR_UIDLEN] = 0;
    t -> gid [TAR_GIDLEN] = 0;
    t -> size [TAR_SIZELEN] = 0;
    t -> mtime [TAR_MTIMELEN] = 0;

    a -> mode = parse_mode (t);
    a -> uid = octal (t -> uid);
    a -> gid = octal (t -> gid);
    a -> nbytes = octal (t -> size);
    a -> mtime = a -> ctime = a -> atime = octal (t -> mtime);
    a -> flags &= ~(IOFUNC_ATTR_ATIME | IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME | IOFUNC_ATTR_DIRTY_TIME);
}

static int
parse_mode (ustar_t *t)
{
    int     mode;

    // GNU tar has their own types in the "typeflag" field...
    mode = octal (t -> mode);
    switch (t -> typeflag) {
    case    REGTYPE:
    case    AREGTYPE:
        mode |= _S_IFREG;
        break;
    case    LNKTYPE:
        mode |= _S_IFREG;
        break;
    case    SYMTYPE:                // Reserved
        mode |= _S_IFLNK;
        break;
    case    CHRTYPE:
        mode |= _S_IFCHR;
        break;
    case    BLKTYPE:
        mode |= _S_IFBLK;
        break;
    case    DIRTYPE:
        mode |= _S_IFDIR;
        break;
    case    FIFOTYPE:
        mode |= _S_IFIFO;
        break;
    case    CONTTYPE:               // Reserved
        mode |= _S_IFREG;
        break;
    }
    return (mode);
}

static void
dump_tar_header (off_t off, ustar_t *t)
{
D {
    if (optv > 1) {
        printf ("DUMP of TAR HEADER AT %p [offset %d]\n", t, off);
        printf ("\tname     \"%s\"\n", t -> name);
        printf ("\tmode     \"%s\"\n", t -> mode);
        printf ("\tuid      \"%s\"\n", t -> uid);
        printf ("\tgid      \"%s\"\n", t -> gid);
        printf ("\tsize     \"%s\"\n", t -> size);
        printf ("\tmtime    \"%s\"\n", t -> mtime);
        printf ("\tcksum    \"%s\"\n", t -> cksum);
        printf ("\ttypeflag %c\n", t -> typeflag);
        printf ("\tlinkname \"%s\"\n", t -> linkname);
        printf ("\tmagic    \"%s\"\n", t -> magic);
        printf ("\tversion  %d %d\n", t -> version [0], t -> version [1]);
        printf ("\tuname    \"%s\"\n", t -> uname);
        printf ("\tgname    \"%s\"\n", t -> gname);
        printf ("\tdevmajor \"%s\"\n", t -> devmajor);
        printf ("\tdevminor \"%s\"\n", t -> devminor);
        printf ("\tprefix   \"%s\"\n", t -> prefix);
    } else {
        printf ("%s\n", t -> name);
    }
  }
}

