
/*
 *  m_main.c
 *
 *  main.c shell/resmgr version 0.006
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
 *  This module represents the main module for the TARFS "mount_tarfs"
 *  helper module.
 *
 *  This program can be invoked by the standard "mount" utility to mount
 *  a .tar filesystem.
 *
 *  2003 01 14  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/mount.h>

#define CP  fprintf (stdout, "[** %s %d **]\n", __FILE__, __LINE__);

static  void    optproc (int, char **);
static  void    single_mount (char *s, char *t);

const   char    *progname = "mount_tarfs";
const   char    *blankname= "           ";
extern  char    *version;                   // version.c

int             optm;                       // multiple
char            *opto;
int             optv;

int
main (int argc, char **argv)
{
    optproc (argc, argv);

    // optproc exits for us
    exit (EXIT_FAILURE);                    // should never get here
}

static void
usageError (void)
{
    fprintf (stderr, "%s:  error in use, type \"use %s\" for usage\n", progname, progname);
    exit (EXIT_FAILURE);
}

static void
optproc (int argc, char **argv)
{
    int     opt;
    char    *target;                        // where to mount the tarfile
    char    *source;                        // the tarfile

    if (!argc) {
        usageError ();
    }

    optm = 0;

    while ((opt = getopt (argc, argv, "mo:v")) != -1) {
        switch (opt) {
        case    'm':
            optm = 1;
            break;
        case    'o':
            opto = optarg;
            if (optv) {
                printf ("%s:  -o %s\n", progname, opto);
            }
            break;
        case    'v':
            optv++;
            if (optv > 1) {
                printf ("Verbosity is %d\n", optv);
            }
            break;
        default:
            usageError ();
            break;
        }
    }

    if (optm) {                                                                 // multiple mount processing; pass each one to the single mount handler
        for (; optind < argc; optind++) {
            single_mount (argv [optind], NULL);
        }
    } else {
        target = source = NULL;

        for (; optind < argc; optind++) {
            if (!source) {
                source = argv [optind];
            } else {
                if (!target) {
                    target = argv [optind];
                } else {
                    fprintf (stderr, "%s: too many arguments!\n", progname);
                    exit (EXIT_FAILURE);
                }
            }
        }
        if (!source) {
            fprintf (stderr, "%s:  we need at least the source (.tar) name!\n", progname);
            exit (EXIT_FAILURE);
        }
        single_mount (source, target);
    }
    exit (EXIT_SUCCESS);
}

static void
single_mount (char *s, char *t)
{
    static  char fulls [_POSIX_PATH_MAX];   // used to resolve the full pathname
    static  char fullt [_POSIX_PATH_MAX];   // and for the target
    int     sts;
    int     l;

    if (*s != '/') {
        if (realpath (s, fulls) == NULL) {
            fprintf (stderr, "%s:  realpath (%s) failed, errno %d (%s)\n", progname, s, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
        s = fulls;
    }
    if (!t || (*t == '/' && !t [1])) {                                          // just the string "/" (bogus; from the "mount" utility, and we never want to mount a tar filesystem as "/"!)
        strcpy (fullt, s);                                                      // put in the full source
        l = strlen (fullt);
        if (l > strlen (".tar")) {                                              // only if it's big enough do we want to play with it...
            if (!strcmp (fullt + l - strlen (".tar"), ".tar")) {                // change .tar -> .dir
                strcpy (fullt + l - strlen (".tar"), ".dir");
            } else if (!strcmp (fullt + l - strlen (".tgz"), ".tgz")) {         // change .tgz -> .dir
                strcpy (fullt + l - strlen (".tgz"), ".dir");
            } else if (!strcmp (fullt + l - strlen (".tar.gz"), ".tar.gz")) {   // change .tar.gz -> .tar.dir
                strcpy (fullt + l - strlen (".gz"), ".dir");
            } else {                                                            // change everything else to + ".dir"
                strcat (fullt, ".dir");
            }
        }
        t = fullt;
    }
    if (optv) {
        printf ("mount_tarfs version %s, source [%s] target [%s]\n", version, s ? s : "<none>", t ? t : "<none>");
    }

    sts = mount (s, t, _MFLAG_OCB, "tarfs", NULL, 0);
    if (sts == -1) {
        fprintf (stderr, "%s:  failed mount (%s, %s, 0, tarfs, NULL, 0), errno %d (%s)\n", progname, s, t, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
}

