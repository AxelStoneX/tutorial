
/*
 *  main.c
 *
 *  main.c shell/resmgr version 0.006
 *
 *  Copyright 2003, 2012 QNX Software Systems Limited. All Rights Reserved.
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
 *  This module represents the main module for the DIO-144 driver.
 *
 *  This program will act as the front-end to the DIO-144 (digital I/O board).
 *
 *  2003 07 03  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/dispatch.h>

#include "iofuncs.h"

static  void    optproc (int, char **);
static  void    execute_resmgr (void);

const   char    *progname = "dio144";
const   char    *blankname= "      ";
extern  char    *version;                   // version.c

int             opt01;                      // default for outputs is 0 or 1?
char            *optc;                      // configuration file
int             optd;                       // debug/diagnostic mode
char            *optn;                      // device name
int             optv;

extern  dio144_t    *dio144;                // dio144.c -- global information structure about all cards (see dio144.h)
extern  int         ndio144;                // dio144.c -- how many are in the structure above

int
main (int argc, char **argv)
{
    if (ThreadCtl (_NTO_TCTL_IO, 0) == -1) {
        fprintf (stderr, "%s:  can't obtain I/O privilege; ensure you are running as root! (errno %d (%s))\n", progname, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    optproc (argc, argv);

    execute_resmgr ();                      // never returns

    return (EXIT_SUCCESS);
}

static void
execute_resmgr (void)
{
    resmgr_attr_t           resmgr_attr;
    resmgr_connect_funcs_t  connect_func;
    resmgr_io_funcs_t       io_func;
    dispatch_t              *dpp;
    resmgr_context_t        *ctp;
    int                     i;

    // create the dispatch structure
    if ((dpp = dispatch_create ()) == NULL) {
        perror ("Unable to dispatch_create()\n");
        exit (EXIT_FAILURE);
    }

    // initialize the various data structures
    memset (&resmgr_attr, 0, sizeof (resmgr_attr));

    // bind default functions into the outcall tables
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);

    // override functions in "connect_func" and "io_func" as required here
    io_func.devctl = io_devctl;

// WARNING:  modify these two if required for your io_devctl() support in iofuncs.c
//  resmgr_attr.nparts_max = 1;
//  resmgr_attr.msg_max_size = 2048;

    // establish a name in the pathname space
    for (i = 0; i < ndio144; i++) {
        if (resmgr_attach (dpp, &resmgr_attr, dio144 [i].name, _FTYPE_ANY, 0, &connect_func, &io_func, &dio144 [i]) == -1) {
            perror ("Unable to resmgr_attach\n");
            exit (EXIT_FAILURE);
        }
    }

    ctp = resmgr_context_alloc (dpp);

    // wait here forever, handling messages
    while (1) {
        if ((ctp = resmgr_block (ctp)) == NULL) {
            perror ("Unable to resmgr_block()\n");
            exit (EXIT_FAILURE);
        }
        resmgr_handler (ctp);
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
    int                 i;
    parser_t            *p;
    parser_handle_t     *ph;

    if (!argc) {
        usageError ();
    }

    opt01 = 0;
    optc = NULL;
    optd = 0;
    optn = NULL;
    dio144 = NULL;
    ndio144 = 0;

    while ((opt = getopt (argc, argv, "01c:dn:p:v")) != -1) {
        switch (opt) {
        case    '0':
            opt01 = 0;
            break;
        case    '1':
            opt01 = 1;
            break;
        case    'c':
            optc = optarg;
            break;
        case    'd':
            optd++;
            break;
        case    'n':
            optn = optarg;
            break;
        case    'p':
            i = strtol (optarg, NULL, 16);
            if (i <= 0 || i > 0xff0) {
                fprintf (stderr, "%s:  -p argument (%s) is out of range 1..ff0 hex\n", progname, optarg);
                exit (EXIT_FAILURE);
            }
            install_card (optn, i);
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
        if (p = parser_parse (ph, "/dev/dio144*")) {
            if (p -> status != PARSER_OK) {
                fprintf (stderr, "%s:  configuration file (%s) error (%s), aborting\n", progname, ph -> fname, parser_strerror (p -> status));
                exit (EXIT_FAILURE);
            }
            if (optv) {
                parser_dump (p);
            }
            install_cards_from_database (p);    // performs validation too
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

    if (ndio144 == 0) {         // nothing added by user, add the default one then
        printf ("No configuration specified; using default at port 0x2c0\n");
        install_card (NULL, 0x2c0);
    }

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }
}

