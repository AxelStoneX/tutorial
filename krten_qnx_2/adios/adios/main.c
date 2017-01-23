
/*
 *  main.c
 *
 *  main.c shell/resmgr version 0.006
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
 *  This module represents the main module for the ADIOS data acquisition
 *  server.
 *
 *  This program will fetch values from various data acquisition cards
 *  and store the data in shared memory.
 *
 *  2003 06 19  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "adios.h"

static  void    optproc (int, char **);
static  void    execute_resmgr (void);

const   char    *progname = "adios";
const   char    *blankname= "     ";
extern  char    *version;                   // version.c

char            *optc;                      // configuration file
const   char    *optn;                      // device name
int             optp;                       // period (ms)
const   char    *opts;                      // shared memory name
int             optS;                       // number of samples in shared memory
int             optv;

extern  int     nadios;                     // adios.c, number of drivers

int
main (int argc, char **argv)
{
    optproc (argc, argv);

    // do anything you need to before becoming a resmgr here...

    create_shmem ();

    pthread_create (NULL, NULL, daq_thread, NULL);

    execute_resmgr ();                      // never returns

    return (EXIT_SUCCESS);
}

static void
execute_resmgr (void)
{
    resmgr_attr_t           resmgr_attr;
    resmgr_connect_funcs_t  connect_func;
    resmgr_io_funcs_t       io_func;
    iofunc_attr_t           attr;
    dispatch_t              *dpp;
    dispatch_context_t      *ctp;

    // create the dispatch structure
    if ((dpp = dispatch_create ()) == NULL) {
        perror ("Unable to dispatch_create()\n");
        exit (EXIT_FAILURE);
    }

    // initialize the various data structures
    memset (&resmgr_attr, 0, sizeof (resmgr_attr));

    // bind default functions into the outcall tables
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);
    iofunc_attr_init (&attr, S_IFREG | 0666, 0, 0);

    // override functions in "connect_func" and "io_func" as required here

    // establish a name in the pathname space
    if (resmgr_attach (dpp, &resmgr_attr, optn, _FTYPE_ANY, 0, &connect_func, &io_func, &attr) == -1) {
        perror ("Unable to resmgr_attach()\n");
        exit (EXIT_FAILURE);
    }

    ctp = dispatch_context_alloc (dpp);

    // wait here forever, handling messages
    while (1) {
        if ((ctp = dispatch_block (ctp)) == NULL) {
            perror ("Unable to resmgr_block()\n");
            exit (EXIT_FAILURE);
        }
        dispatch_handler (ctp);
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
    int                 opt;
    int                 did_any;
    parser_t            *p;
    parser_handle_t     *ph;

    if (!argc) {
        usageError ();
    }

    optc = NULL;
    optn = "/dev/adios";
    optp = 100;
    opts = "/adios";
    optS = 1000;

    did_any = 0;

    while ((opt = getopt (argc, argv, "c:n:p:s:S:v")) != -1) {
        switch (opt) {
        case    'c':
            optc = optarg;
            break;
        case    'n':
            optn = optarg;
            break;
        case    'p':
            optp = atoi (optarg);
            if (optp < 1) {
                fprintf (stderr, "%s:  period (-p) must be greater than or equal to 1 millisecond\n", progname);
                exit (EXIT_FAILURE);
            }
            break;
        case    's':
            opts = optarg;
            break;
        case    'S':
            optS = atoi (optarg);
            if (optS < 1) {
                fprintf (stderr, "%s:  number of samples (-S) must be greater than or equal to 1\n", progname);
                exit (EXIT_FAILURE);
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

    if (ph = parser_open (optc)) {
        if (p = parser_parse (ph, "*")) {
            if (p -> status != PARSER_OK) {
                fprintf (stderr, "%s:  configuration file (%s) error (%s), aborting\n", progname, ph -> fname, parser_strerror (p -> status));
                exit (EXIT_FAILURE);
            }
            if (optv) {
                parser_dump (p);
            }
            install_drivers_from_database (p);  // performs validation too
            did_any++;
            parser_free (p);            // we're done with it, kill it
        }
        parser_close (ph);
    } else {
        if (optc) {
            // if they specified a configuration file and we couldn't open it, exit
            printf ("Didn't find configuration file \"%s\"; aborting\n", optc);
            exit (EXIT_FAILURE);
        } else {
            // if they didn't specify a configuration file, just ignore it.
            printf ("Didn't find a configuration file; no configuration file used\n");
        }
    }

    // allow them to add extra devices on the command line
    for (; optind < argc; optind++) {
        if (add_device (argv [optind], -1)) {
            if (optv) {
                printf ("%s%s installed\n", did_any ? "Additional device" : "Device", argv [optind]);
            }
        }
    }

    // see if they got any devices!
    if (!nadios) {
        fprintf (stderr, "%s:  need to specify at least one device!\n", progname);
        exit (EXIT_FAILURE);
    }
}

