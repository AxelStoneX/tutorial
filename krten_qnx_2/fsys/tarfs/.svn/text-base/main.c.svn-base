
/*
 *  main.c
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
 *  This module represents the main module for the TARFS
 *  cfs example program.
 *
 *  This program will manifest a tar filesystem, using the cfs
 *  filesystem library.
 *
 *  2002 08 13  R. Krten        created
 *  2003 01 12  R. Krten        swiped from ramdisk
 *  2003 10 23  R. Krten        released for publicatoin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <sys/dispatch.h>

#include "cfs.h"
#include <sys/iofunc.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <inttypes.h>

static  void    optproc (int, char **);
static  void    execute_resmgr (void);

const   char    *progname = "tarfs";
const   char    *blankname= "     ";
extern  char    *version;                   // version.c

int             optd;                       // debug flag
const   char    *optn;                      // device name (name of tarfs "device", not usable without a "mount" command)
int             optm, optM;                 // maximum cache sizes
int             optt;
int             optv;

mpool_t         *mpool_block;               // global -- holds the cache blocks
mpool_t         *mpool_iov8;                // global -- holds 8 iov_t's
mpool_t         *mpool_readdir;             // global -- holds 2k readdir buffer

resmgr_connect_funcs_t  connect_func;       // connect funcs
resmgr_io_funcs_t       io_func;            // io funcs
dispatch_t              *dpp;               // dispatch structure
resmgr_attr_t           resmgr_attr;        // attributes for resource manager

int
main (int argc, char **argv)
{
    optproc (argc, argv);

    // do anything you need to before becoming a resmgr here...

    execute_resmgr ();                      // never returns

    return (EXIT_SUCCESS);
}

static void
execute_resmgr (void)
{
    resmgr_context_t        *ctp;
    iofunc_mount_t          mnt;
    resmgr_connect_funcs_t  mount_only_cfuncs;  // everything is defaulted except io_mount() outcall

    // for timing analysis
    uint64_t                t_start, t_stop;
    int                     msgtype;
    double                  factor;         // for conversion to microseconds

    // cfs add-ons
    cfs_attr_t              cfs_attr;       // must be persistent

    // create the dispatch structure
    if ((dpp = dispatch_create ()) == NULL) {
        perror ("Unable to dispatch_create()\n");
        exit (EXIT_FAILURE);
    }

    // initialize the various data structures
    memset (&resmgr_attr, 0, sizeof (resmgr_attr));
    memset (&mnt, 0, sizeof (mnt));
    resmgr_attr.nparts_max = 2;                                                     // at least 2 for symlink redirect

    // bind default functions into the outcall tables
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);
    iofunc_attr_init (&cfs_attr.attr, S_IFNAM | 0555, 0, 0);

    /*
     *  CFS modifications to standard "/dev/null" template
    */

    // mount magic
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &mount_only_cfuncs, 0, NULL);
    mount_only_cfuncs.mount = cfs_c_mount;
    mnt.conf = IOFUNC_PC_SYNC_IO;
    cfs_attr.attr.mount = &mnt;
    mnt.dev = 1;

    chdir ("/"); // without this, you can't create mountpoint -- prolly need to add a "/" in front of the resmgr_attach in c_mount.c

    mpool_block = mpool_create (BLOCKSIZE, max (optm, optM) / BLOCKSIZE, optM ? MPOOL_TYPE_STATIC : MPOOL_TYPE_DYNAMIC, "block pool");
    if (mpool_block == NULL) {
        fprintf (stderr, "%s:  unable to mpool_create \"block pool\" (%d, %d)\n", progname, BLOCKSIZE, max (optm, optM));
        exit (EXIT_FAILURE);
    }

    mpool_iov8 = mpool_create (sizeof (iov_t) * 8, 64, MPOOL_TYPE_STATIC, "iov8 pool");
    if (mpool_iov8 == NULL) {
        fprintf (stderr, "%s:  unable to mpool_create \"iov8 pool\" (%d, %d)\n", progname, sizeof (iov_t) * 8, 64);
        exit (EXIT_FAILURE);
    }

    mpool_readdir = mpool_create (2048, 8, MPOOL_TYPE_STATIC, "readdir pool");
    if (mpool_readdir == NULL) {
        fprintf (stderr, "%s:  unable to mpool_create \"readdir pool\" (%d, %d)\n", progname, 2048, 8);
        exit (EXIT_FAILURE);
    }

    cfs_func_init (&connect_func, &io_func);                            // overwrites connect_func and io_func
    cfs_attr_init (&cfs_attr);                                          // initializes extended attribute structure
    cfs_attr.attr.inode = (int) &cfs_attr;                              // set up the inode

    // establish a mountable entity (this does *not* have a name!)
    if (resmgr_attach (dpp, &resmgr_attr, NULL, _FTYPE_MOUNT, _RESMGR_FLAG_DIR | _RESMGR_FLAG_FTYPEONLY, &mount_only_cfuncs, &io_func, &cfs_attr) == -1) {
        perror ("Unable to resmgr_attach()\n");
        exit (EXIT_FAILURE);
    }

    /*
     *  end of CFS modifications
    */

    ctp = resmgr_context_alloc (dpp);

    // set up factor for conversion to microseconds
    factor = 1./ ((double) SYSPAGE_ENTRY(qtime)->cycles_per_sec / 1e6);

    // wait here forever, handling messages
    while (1) {
        if ((ctp = resmgr_block (ctp)) == NULL) {
            perror ("Unable to resmgr_block()\n");
            exit (EXIT_FAILURE);
        }
        msgtype = ctp -> msg -> type;
        t_start = ClockCycles ();
        resmgr_handler (ctp);
        t_stop = ClockCycles ();

        if (optt) {
            printf ("Message 0X%04X took %g us\n", msgtype, (double) (t_stop - t_start) * factor);
        }
    }
}

static void
usageError (void)
{
    fprintf (stderr, "%s:  error in use, type \"use %s\" for usage\n", progname, progname);
    exit (1);
}

static void
optproc (int argc, char **argv)
{
    int     opt;
    double  floater;
    int     c;

    if (!argc) {
        usageError ();
    }

    optd = 0;               // no debug
    optn = "/dev/tarfs";
    optm = optM = 0;
    optt = 0;

    while ((opt = getopt (argc, argv, "dm:M:n:tv")) != -1) {
        switch (opt) {
        case    'd':
            optd++;
            break;
        case    'm':
        case    'M':
            floater = atof (optarg);
            switch (c = optarg [strlen (optarg) - 1]) {
            case    'M':
                floater *= 1024;
                // fall through
            case    'k':
                floater *= 1024;
                break;
            default:
                if (!isdigit (c)) {
                    fprintf (stderr, "%s:  invalid -%c trailer '%c' (valid are 'k' or 'M')\n", progname, opt, c);
                    exit (EXIT_FAILURE);
                }
                // if it's a digit it's fine...
                break;
            }
            if (floater < 4096) {
                fprintf (stderr, "%s:  number of cache blocks too small (must be at least 4k)\n", progname);
                exit (EXIT_FAILURE);
            }
            if (opt == 'm') {
                optm = floater;
            } else {
                optM = floater;
            }
D           printf ("Selected maximum cache block size is %d bytes\n", optm ? optm : optM);
            break;
        case    'n':
            optn = optarg;
            break;
        case    't':
            optt++;
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

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }

    if (optv) {
        printf ("TARFS version %s\n", version);
    }
}

