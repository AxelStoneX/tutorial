
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
 *  This module represents the main module for the web counter phase I.
 *
 *  This program will manifest a graphic image representing a web
 *  counter as a demonstration of generating data on-demand.
 *  Later phases build on this concept.
 *
 *  2003 03 25  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "gif.h"
#include "7seg.h"

struct my_attr_s;
struct my_ocb_s;
#define IOFUNC_ATTR_T   struct my_attr_s
#define IOFUNC_OCB_T    struct my_ocb_s

#include <sys/iofunc.h>

typedef struct my_attr_s
{
    iofunc_attr_t       base;
    int                 count;
}   my_attr_t;

typedef struct my_ocb_s
{
    iofunc_ocb_t        base;
    unsigned char       *output;                // output of GIF convertor
    int                 size;                   // the size of the resource
}   my_ocb_t;

#include <sys/dispatch.h>

#define MAX_DIGITS  10                          // maximum number of digits we can display

static  void    optproc (int, char **);
static  void    execute_resmgr (void);

static  int     io_close_ocb (resmgr_context_t *ctp, void *reserved, RESMGR_OCB_T *ocb);
static  int     io_open (resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
static  int     io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);

const   char    *progname = "webcounter1";
const   char    *blankname= "           ";
extern  char    *version;                       // version.c

        int         optb;                       // background colour (default 0x000000 == black)
static  int         optd;                       // number of digits (default 6)
        int         optf;                       // foreground colour (default 0xff0000 == red; they're LED's after all)
static  const char  *optn;                      // device name (default "/dev/webcounter.gif")
static  int         opts;                       // start count (and current count value) (default 0)
static  int         optv;                       // verbosity
static  int         optx;                       // X size (default optd * 10)
static  int         opty;                       // Y size (default 20)

int
main (int argc, char **argv)
{
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
    my_attr_t               attr;
    dispatch_t              *dpp;
    resmgr_context_t        *ctp;

    // create the dispatch structure
    if ((dpp = dispatch_create ()) == NULL) {
        perror ("Unable to dispatch_create()\n");
        exit (EXIT_FAILURE);
    }

    // initialize the various data structures
    memset (&resmgr_attr, 0, sizeof (resmgr_attr));
    memset (&attr, 0, sizeof (attr));

// WARNING:  modify these two if required for your io_devctl() support in iofuncs.c
//  resmgr_attr.nparts_max = 1;
//  resmgr_attr.msg_max_size = 2048;

    // bind default functions into the outcall tables
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);
    iofunc_attr_init (&attr.base, S_IFREG | 0666, 0, 0);

    // override functions in "connect_func" and "io_func" as required here
    connect_func.open = io_open;
    io_func.read      = io_read;
    io_func.close_ocb = io_close_ocb;

    // establish a name in the pathname space
    if (resmgr_attach (dpp, &resmgr_attr, optn, _FTYPE_ANY, 0, &connect_func, &io_func, &attr) == -1) {
        perror ("Unable to resmgr_attach()\n");
        exit (EXIT_FAILURE);
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
    int     opt;

    if (!argc) {
        usageError ();
    }

    optb = 0;
    optd = 6;
    optf = 0xff0000;
    optn = "/dev/webcounter.gif";
    opts = 0;
    optx = -1;
    opty = -1;

    while ((opt = getopt (argc, argv, "b:d:f:n:s:x:y:v")) != -1) {
        switch (opt) {
        case    'b':
            optb = strtol (optarg, NULL, 16);
            if (optb < 0 || optb > 0xffffff) {
                fprintf (stderr, "%s:  background colour is out of range {0..ffffff} (RRGGBB hexadecimal)\n", progname);
                exit (EXIT_FAILURE);
            }
            break;
        case    'd':
            optd = atoi (optarg);
            if (optd < 2 || optd > MAX_DIGITS) {
                fprintf (stderr, "%s:  -d value is out of range {1..%d}\n", progname, MAX_DIGITS);
                exit (EXIT_FAILURE);
            }
            break;
        case    'f':
            optf = strtol (optarg, NULL, 16);
            if (optf < 0 || optf > 0xffffff) {
                fprintf (stderr, "%s:  foreground colour is out of range {0..ffffff} (RRGGBB hexadecimal)\n", progname);
                exit (EXIT_FAILURE);
            }
            break;
        case    'n':
            optn = optarg;
            break;
        case    's':
            opts = atoi (optarg);               // no error checking.  You wanna start it negative?  Go for it.
            break;
        case    'v':
            optv++;
            if (optv > 1) {
                printf ("Verbosity is %d\n", optv);
            }
            break;
        case    'x':
            optx = atoi (optarg);               // we'll error check this when we have the digit count
            break;
        case    'y':
            opty = atoi (optarg);
            break;
        default:
            usageError ();
            break;
        }
    }

    if (optx == -1) {
        optx = 10 * optd;
    } else {
        if (optx < (3 * optd)) {
            fprintf (stderr, "%s:  X size (-x option) is too small.  We need at least 3 pixels per digit, there are %d digits, so we need %d pixels, not the %d you have given\n", progname, optd, optd * 3, optx);
            exit (EXIT_FAILURE);
        }
    }
    if (opty == -1) {
        opty = 20;
    }

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }
}

/*
 *  io_read
 *
 *  A fairly straightforward implementation of io_read, with one minor
 *  twist -- the size of the resource.
 *
 *  The resource needs to have the correct size associated with it.
 *  Different images will generate differently-sized compressed GIF
 *  streams.
 *
 *  Therefore, what we do is we set the size of the resource in io_open()
 *  to be the maximum size it can be.  This gets set into the attribute
 *  structure's nbytes member, and effectively serves as the "default"
 *  size.
 *
 *  In functions that care about the *real* size (i.e., io_read()), we
 *  use the size from ocb -> size, which is the actual size of the
 *  compressed stream.
 *
 *  Effectively, the attributes structure contains the maximum size,
 *  and io_read() further limits the size to something smaller.
*/

static int
io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int     nbytes;
    int     sts;
    char    string [MAX_DIGITS + 1];

    // we don't do any xtypes here...
    if ((msg -> i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return (ENOSYS);
    }

    // standard helper
    if ((sts = iofunc_read_verify (ctp, msg, &ocb -> base, NULL)) != EOK) {
        return (sts);
    }

    // allocate buffer if required (including temporary "input" buffer)
    if (!ocb -> output) {
        unsigned char *input;           // limited scope

        if (!(input = calloc (optx, opty))) {
            return (ENOMEM);
        }
        if (!(ocb -> output = calloc (optx, opty))) {
            free (input);
            return (ENOMEM);
        }

        sprintf (string, "%0*d", optd, ocb -> base.attr -> count++);
        render_7segment (string, input, optx, opty);
        ocb -> size = encode_image (input, optx, opty, ocb -> output);
        free (input);
    }

    // figure out how many we can return to the client
    nbytes = min (ocb -> size - ocb -> base.offset, msg -> i.nbytes);

    if (nbytes) {
        // return it to the client
        MsgReply (ctp -> rcvid, nbytes, ocb -> output + ocb -> base.offset, nbytes);

        // update flags and offset
        ocb -> base.attr -> base.flags |= IOFUNC_ATTR_ATIME | IOFUNC_ATTR_DIRTY_TIME;
        ocb -> base.offset += nbytes;
    } else {
        // nothing to return, indicate End Of File
        MsgReply (ctp -> rcvid, EOK, NULL, 0);
    }

    // already done the reply ourselves
    return (_RESMGR_NOREPLY);
}

static int
io_open (resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra)
{
    IOFUNC_OCB_T    *ocb;
    int     sts;

    sts = iofunc_open (ctp, msg, &handle -> base, NULL, NULL);
    if (sts != EOK) {
        return (sts);
    }

    ocb = calloc (1, sizeof (*ocb));
    if (ocb == NULL) {
        return (ENOMEM);
    }

    handle -> base.nbytes = optx * opty;                // give them the unencoded ("default") size

    sts = iofunc_ocb_attach (ctp, msg, &ocb -> base, &handle -> base, NULL);
    return (sts);
}

static int
io_close_ocb (resmgr_context_t *ctp, void *reserved, RESMGR_OCB_T *ocb)
{
    if (ocb -> output) {
        free (ocb -> output);
        ocb -> output = NULL;
    }

    return (iofunc_close_ocb_default (ctp, reserved, &ocb -> base));
}

