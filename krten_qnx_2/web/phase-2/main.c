
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
 *  This module represents the main module for the web counter phase II.
 *
 *  This program will manifest a graphic image representing a web
 *  counter as a demonstration of generating data on-demand.
 *
 *  2003 03 25  R. Krten        created
 *  2003 08 12  R. Krten        added io_write()
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#include "gif.h"
#include "7seg.h"
#include "8x8.h"

#define MAX_DIGITS  10                          // maximum number of digits we can display

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
    char                wbuf [MAX_DIGITS + 2];  // we store the incoming write() from the client here.
    int                 size;                   // we need to store it here because each one is a different size!
    void                (*render) (char *string, unsigned char *bitmap, int x, int y);
}   my_ocb_t;

#include <sys/dispatch.h>

static  void    optproc (int, char **);
static  void    execute_resmgr (void);

static  int     io_close_ocb (resmgr_context_t *ctp, void *reserved, RESMGR_OCB_T *ocb);
static  int     io_open (resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
static  int     io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
static  int     io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);

static  int     read_file (char *fname);
static  void    write_file (char *fname, int val);

static  void    convert_gif_to_txt_filename (const char *gif, char *txt);

const   char    *progname = "webcounter2";
const   char    *blankname= "           ";
extern  char    *version;                       // version.c

        int         optb;                       // background colour (default 0x000000 == black)
static  int         optd;                       // number of digits (default 6)
        int         optf;                       // foreground colour (default 0xff0000 == red; they're LED's after all)
static  const char  *optn;                      // device name (default "/dev/webcounter.gif")
static  void        (*optr) (char *string, unsigned char *bitmap, int x, int y);    // render function (default from command line)
static  int         opts;                       // start count (and current count value) (default 0)
static  char        *optS;                      // counter storage file
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

static  my_attr_t               attr_gif;
static  my_attr_t               attr_txt;

static void
execute_resmgr (void)
{
    resmgr_attr_t           resmgr_attr;
    resmgr_connect_funcs_t  connect_func;
    resmgr_io_funcs_t       io_func;
    dispatch_t              *dpp;
    resmgr_context_t        *ctp;
    char                    txtname [BUFSIZ];

    // create the dispatch structure
    if ((dpp = dispatch_create ()) == NULL) {
        perror ("Unable to dispatch_create()\n");
        exit (EXIT_FAILURE);
    }

    // initialize the various data structures
    memset (&resmgr_attr, 0, sizeof (resmgr_attr));
    memset (&attr_gif, 0, sizeof (attr_gif));
    memset (&attr_txt, 0, sizeof (attr_txt));

    attr_gif.count = opts;
    attr_txt.count = opts;

// WARNING:  modify these two if required for your io_devctl() support in iofuncs.c
//  resmgr_attr.nparts_max = 1;
//  resmgr_attr.msg_max_size = 2048;

    // bind default functions into the outcall tables
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);
    iofunc_attr_init (&attr_gif.base, S_IFREG | 0666, 0, 0);
    iofunc_attr_init (&attr_txt.base, S_IFREG | 0666, 0, 0);

    // override functions in "connect_func" and "io_func" as required here
    connect_func.open = io_open;
    io_func.read      = io_read;
    io_func.close_ocb = io_close_ocb;
    io_func.write     = io_write;

    // establish a name in the pathname space for the .GIF file:
    if (resmgr_attach (dpp, &resmgr_attr, optn, _FTYPE_ANY, 0, &connect_func, &io_func, &attr_gif) == -1) {
        perror ("Unable to resmgr_attach()\n");
        exit (EXIT_FAILURE);
    }

    // establish a name in the pathname space for the text file:
    convert_gif_to_txt_filename (optn, txtname);
    if (resmgr_attach (dpp, &resmgr_attr, txtname, _FTYPE_ANY, 0, &connect_func, &io_func, &attr_txt) == -1) {
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
    optr = render_7segment;
    opts = -1;
    optS = NULL;
    optx = -1;
    opty = -1;

    while ((opt = getopt (argc, argv, "b:d:f:n:r:s:S:x:y:v")) != -1) {
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
        case    'r':
            if (!strcmp (optarg, "8x8")) {
                optr = render_8x8;
            } else if (!strcmp (optarg, "7seg")) {
                optr = render_7segment;
            } else {
                fprintf (stderr, "%s:  bad value for -r, valid {8x8, 7seg}\n", progname);
                exit (EXIT_FAILURE);
            }
            break;
        case    's':
            opts = atoi (optarg);               // no error checking.  You wanna start it negative?  Go for it.
            break;
        case    'S':
            optS = optarg;
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

    if (opts == -1) {
        opts = 0;
        if (optS != NULL) {
            opts = read_file (optS);
        }
    }

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }
}

static int
io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int     nbytes;
    int     nleft;
    int     sts;
    char    string [MAX_DIGITS + 2];    // +1 for \n, +1 for nul terminator

    // we don't do any xtypes here...
    if ((msg -> i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return (ENOSYS);
    }

    // use the helper function to decide if valid
    if ((sts = iofunc_read_verify (ctp, msg, &ocb -> base, NULL)) != EOK) {
        return (sts);
    }

    if (ocb -> base.attr == &attr_gif) {
        unsigned char *input;           // limited scope

        if (!ocb -> output) {
            input = calloc (optx, opty);
            if (input == NULL) {
                return (ENOMEM);
            }
            ocb -> output = calloc (optx, opty);
            if (ocb -> output == NULL) {
                free (input);
                return (ENOMEM);
            }
    
            sprintf (string, "%0*d", optd, ocb -> base.attr -> count++);
            (*ocb -> render) (string, input, optx, opty);
            ocb -> size = encode_image (input, optx, opty, ocb -> output);
            free (input);
    
            if (optS) {
                write_file (optS, ocb -> base.attr -> count);
            }
        }
    } else {        // must be the text attribute
        ocb -> base.attr -> count = attr_gif.count;
        sprintf (string, "%0*d\n", optd, ocb -> base.attr -> count);
        if (ocb -> output) {
            free (ocb -> output);
        }
        ocb -> output = (unsigned char *) strdup (string);
        ocb -> size = strlen ( (const char *) ocb -> output) + 1;
    }

    // figure out how many bytes are left
    nleft = ocb -> size - ocb -> base.offset;

    // and how many we can return to the client
    nbytes = min (nleft, msg -> i.nbytes);

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
io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
    int     nroom;
    int     nbytes;
    int     sts;

    // we don't do any xtypes here...
    if ((msg -> i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return (ENOSYS);
    }

    // use the helper function to decide if valid
    if ((sts = iofunc_write_verify (ctp, msg, &ocb -> base, NULL)) != EOK) {
        return (sts);
    }

    // figure out how many bytes we can accept in total
    nroom = sizeof (ocb -> wbuf) - 1 - ocb -> base.offset;

    // and how many we can accept from the client
    nbytes = min (nroom, msg -> i.nbytes);

    if (nbytes) {
        memcpy (ocb -> wbuf + ocb -> base.offset, &msg -> i + 1, nbytes);

        // update flags and offset
        ocb -> base.attr -> base.flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_DIRTY_TIME;
        ocb -> base.offset += nbytes;
    } else {
        return (ENOSPC);
        MsgReply (ctp -> rcvid, EOK, NULL, 0);
    }

    _IO_SET_WRITE_NBYTES (ctp, nbytes);
    return (EOK);
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

    ocb -> render = optr;
    if (handle == &attr_txt) {
        handle -> base.nbytes = optd + 2;                   // give them the text size
    } else {
        handle -> base.nbytes = optx * opty;                // give them the unencoded size
    }

    sts = iofunc_ocb_attach (ctp, msg, &ocb -> base, &handle -> base, NULL);
    return (sts);
}

static int
io_close_ocb (resmgr_context_t *ctp, void *reserved, RESMGR_OCB_T *ocb)
{
    int     tmp;

    if (ocb -> output) {
        free (ocb -> output);
        ocb -> output = NULL;
    }

    // if we were writing, parse the input buffer and possibly adjust the count
    if (ocb -> base.ioflag & _IO_FLAG_WR) {
        ocb -> wbuf [optd + 1] = 0; // ensure nul terminated and correct size
        if (isdigit (*ocb -> wbuf)) {
            attr_gif.count = attr_txt.count = tmp = atoi (ocb -> wbuf);
            if (optS) {
                write_file (optS, tmp);
            }
        }
    }

    return (iofunc_close_ocb_default (ctp, reserved, &ocb -> base));
}

static int
read_file (char *fname)
{
    FILE    *fp;
    char    buf [MAX_DIGITS + 1];   // +1 for \n

    if ((fp = fopen (fname, "r")) == NULL) {
        return (0);
    }
    *buf = 0;
    fgets (buf, sizeof (buf), fp);
    fclose (fp);
    return (atoi (buf));
}

static void
write_file (char *fname, int val)
{
    FILE    *fp;

    if ((fp = fopen (fname, "w")) == NULL) {
        return;
    }
    fprintf (fp, "%d\n", val);
    fclose (fp);
}

static void
convert_gif_to_txt_filename (const char *gif, char *txt)
{
    if (strlen (gif) < 4) {
        sprintf (txt, "%s.txt", gif);
        return;
    }

    if (!strcmp (gif + strlen (gif) - 4, ".gif")) {
        strcpy (txt, gif);
        sprintf (txt + strlen (txt) - 4, ".txt");
        return;
    }

    sprintf (txt, "%s.txt", gif);
}

