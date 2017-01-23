
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
 *  This module represents the main module for the web counter phase III.
 *
 *  This program will manifest a graphic image representing a web
 *  counter as a demonstration of generating data on-demand.
 *  In Phase III of the project we converted the individual file registrations
 *  to use a directory instead of a file.
 *
 *  2003 03 25  R. Krten        created
 *  2003 08 12  R. Krten        added io_write()
 *  2003 08 12  R. Krten        phase III
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
#include <dirent.h>

#include "gif.h"
#include "7seg.h"
#include "8x8.h"

#define CP              fprintf (stdout, "%s %d\n", __FILE__, __LINE__); fflush (stdout);
#define MAX_DIGITS                  10                      // maximum number of digits we can display

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
static  int     io_read_dir (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
static  int     io_read_file (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
static  int     io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);

static  void    read_file (char *fname);
static  void    write_file (char *fname, int counter, int val);

const   char    *progname = "webcounter3";
const   char    *blankname= "           ";
extern  char    *version;                       // version.c

        int         optb;                       // background colour (default 0x000000 == black)
static  int         optd;                       // number of digits (default 6)
        int         optf;                       // foreground colour (default 0xff0000 == red; they're LED's after all)
static  const char  *optn;                      // device name (default "/dev/webcounter.gif")
static  int         optN;                       // number of counters (default 100)
static  int         optNsize;                   // number of digits in 0..optN-1
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

static  my_attr_t               attr;                       // the base attribute for the directory
static  my_attr_t               *attr_gif;                  // individual attributes for each web counter's .gif file
static  my_attr_t               *attr_txt;                  // individual attributes for each web counter's .txt file

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

    // initialize the individual attributes structures
    // note that an inode of 0 is considered as "not in use"
    // so our inodes go from 2..optN * 2 - 1 (optN for the directory)
    for (i = 0; i < optN; i++) {
        iofunc_attr_init (&attr_gif [i].base, S_IFREG | 0666, 0, 0);
        iofunc_attr_init (&attr_txt [i].base, S_IFREG | 0666, 0, 0);
        attr_txt [i].base.inode = (i + 1) * 2;              // even inodes are TXT files
        attr_gif [i].base.inode = (i + 1) * 2 + 1;          // odd inodes are GIF files
    }

    // initialize the directory inode
    iofunc_attr_init (&attr.base, S_IFDIR | 0777, 0, 0);
    attr.base.inode = 1;                                    // our directory has an inode of one.

    // bind default functions into the outcall tables
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);

    // override functions in "connect_func" and "io_func" as required here
    connect_func.open = io_open;
    io_func.read      = io_read;
    io_func.close_ocb = io_close_ocb;
    io_func.write     = io_write;

    // establish a name in the pathname space for the directory
    if (resmgr_attach (dpp, &resmgr_attr, optn, _FTYPE_ANY, _RESMGR_FLAG_DIR, &connect_func, &io_func, &attr) == -1) {
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
    int     i;
    char    tmp [12];           // 2^31 - 1 is the largest number of counters, which is 9 digits :-)

    if (!argc) {
        usageError ();
    }

    optb = 0;
    optd = 6;
    optf = 0xff0000;
    optn = "/dev/webcounters";
    optN = 100;
    optr = render_7segment;
    opts = -1;
    optS = NULL;
    optx = -1;
    opty = -1;

    while ((opt = getopt (argc, argv, "b:d:f:n:N:r:s:S:x:y:v")) != -1) {
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
        case    'N':
            optN = atoi (optarg);
            if (optN < 1) {
                fprintf (stderr, "%s:  -N value is out of range {0..2^31-1}\n", progname);
                exit (EXIT_FAILURE);
            }
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

    // allocate the individual attributes structures
    // we do that here because we need these to be valid before we call read_file()
    attr_gif = calloc (optN, sizeof (my_attr_t));
    attr_txt = calloc (optN, sizeof (my_attr_t));
    if (!attr_gif || !attr_txt) {
        fprintf (stderr, "%s:  insufficient memory to allocate %d counters\n", progname, optN);
        exit (EXIT_FAILURE);
    }

    if (opts == -1) {
        opts = 0;
        if (optS != NULL) {
            read_file (optS);
        }
    } else {
        for (i = 0; i < optN; i++) {
            attr_gif [i].count = attr_txt [i].count = opts;
        }
    }

    // calculate the number of digits we need based on the maximum number of counters
    optNsize = sprintf (tmp, "%d", optN - 1);

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }
}

static int
io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int     sts;

    // use the helper function to decide if valid
    if ((sts = iofunc_read_verify (ctp, msg, &ocb -> base, NULL)) != EOK) {
        return (sts);
    }

    // decide if we should perform the "file" or "dir" read
    if (S_ISDIR (ocb -> base.attr -> base.mode)) {
        return (io_read_dir (ctp, msg, ocb));
    } else if (S_ISREG (ocb -> base.attr -> base.mode)) {
        return (io_read_file (ctp, msg, ocb));
    } else {
        return (EBADF);
    }
}

#define ALIGN(x) (((x) + 3) & ~3)

static int
dirent_size (char *fname)
{
    return (ALIGN (sizeof (struct dirent) - 4 + strlen (fname)));
}

static struct dirent *
dirent_fill (struct dirent *dp, int inode, int offset, char *fname)
{
    dp -> d_ino = inode;
    dp -> d_offset = offset;
    strcpy (dp -> d_name, fname);
    dp -> d_namelen = strlen (dp -> d_name);
    dp -> d_reclen = ALIGN (sizeof (struct dirent) - 4 + dp -> d_namelen);
    return ((struct dirent *) ((char *) dp + dp -> d_reclen));
}

static int
io_read_dir (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int     nbytes;
    int     nleft;
    struct  dirent *dp;
    char    *reply_msg;
    char    fname [PATH_MAX];

    // allocate a buffer for the reply
    reply_msg = calloc (1, msg -> i.nbytes);
    if (reply_msg == NULL) {
        return (ENOMEM);
    }

    // assign output buffer
    dp = (struct dirent *) reply_msg;

    // we have "nleft" bytes left
    nleft = msg -> i.nbytes;
    while (ocb -> base.offset < optN * 2) {

        // create the filename
        if (ocb -> base.offset & 1) {
            sprintf (fname, "counter-%0*d.gif", optNsize, (int) (ocb -> base.offset / 2));
        } else {
            sprintf (fname, "counter-%0*d.txt", optNsize, (int) (ocb -> base.offset / 2));
        }

        // see how big the result is
        nbytes = dirent_size (fname);

        // do we have room for it?
        if (nleft - nbytes >= 0) {

            // fill the dirent, and advance the dirent pointer
            dp = dirent_fill (dp, ocb -> base.offset + 2, ocb -> base.offset, fname);

            // move the OCB offset
            ocb -> base.offset++;

            // account for the bytes we just used up
            nleft -= nbytes;
        } else {

            // don't have any more room, stop
            break;
        }
    }

    // return info back to the client
    MsgReply (ctp -> rcvid, (char *) dp - reply_msg, reply_msg, (char *) dp - reply_msg);

    // release our buffer
    free (reply_msg);

    // tell resource manager library we already did the reply
    return (_RESMGR_NOREPLY);
}

static int
io_read_file (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int     nbytes;
    int     nleft;
    char    string [MAX_DIGITS + 2];    // +1 for \n, +1 for nul terminator

    // we don't do any xtypes here...
    if ((msg -> i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return (ENOSYS);
    }

    if (ocb -> base.attr -> base.inode & 1) {   // odd inodes are GIF files
        unsigned char *input;

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
                write_file (optS, ocb -> base.attr -> base.inode / 2 - 1, ocb -> base.attr -> count);
            }
        }
    } else {        // must be the text attribute
        int     tmp;

        ocb -> base.attr -> count = attr_gif [ocb -> base.attr -> base.inode / 2 - 1].count;
        tmp = sprintf (string, "%0*d\n", optd, ocb -> base.attr -> count);
        if (ocb -> output) {
            free (ocb -> output);
        }
        ocb -> output = (unsigned char *) strdup (string);
        ocb -> size = tmp;
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
    int             sts;
    int             counter;

    sts = iofunc_open (ctp, msg, &handle -> base, NULL, NULL);
    if (sts != EOK) {
        return (sts);
    }

    ocb = calloc (1, sizeof (*ocb));
    if (ocb == NULL) {
        return (ENOMEM);
    }

    if (!msg -> connect.path [0]) {                 // if it's the directory
        handle -> base.nbytes = optN * 2;           // give them the number of entries
    } else {                                        // else it's one of the files within the directory
        if (strncmp (msg -> connect.path, "counter-", strlen ("counter-"))) {
            return (ENOENT);                        // must start with "counter-"
        }
        counter = atoi (msg -> connect.path + strlen ("counter-"));
        if (counter < 0 || counter >= optN) {
            return (ENOENT);
        }
        if (!strncmp (msg -> connect.path + strlen ("counter-") + optNsize + 1, "gif", 3)) {
            handle = &attr_gif [counter];
            handle -> base.nbytes = optx * opty;    // give them the unencoded size
        } else {
            handle = &attr_txt [counter];
            handle -> base.nbytes = optd + 2;       // give them the text size
        }
        ocb -> render = optr;
    }

    sts = iofunc_ocb_attach (ctp, msg, &ocb -> base, &handle -> base, NULL);
    return (sts);
}

static int
io_close_ocb (resmgr_context_t *ctp, void *reserved, RESMGR_OCB_T *ocb)
{
    int     tmp;

    // free up our allocated resources
    if (ocb -> output) {
        free (ocb -> output);
        ocb -> output = NULL;
    }

    // if we were writing, parse the input buffer and possibly adjust the count
    if (ocb -> base.ioflag & _IO_FLAG_WR) {
        ocb -> wbuf [optd + 1] = 0; // ensure nul terminated and correct size
        if (isdigit (*ocb -> wbuf)) {
            attr_gif [ocb -> base.attr -> base.inode / 2 - 1].count = 
            attr_txt [ocb -> base.attr -> base.inode / 2 - 1].count =
            tmp = atoi (ocb -> wbuf);
            if (optS) {
                write_file (optS, ocb -> base.attr -> base.inode / 2 - 1, tmp);
            }
        }
    }

    return (iofunc_close_ocb_default (ctp, reserved, &ocb -> base));
}

static void
read_file (char *fname)
{
    FILE    *fp;
    int     *counters;
    int     nbytes;
    int     i;

    if ((fp = fopen (fname, "r")) == NULL) {
        fprintf (stderr, "%s:  couldn't open %s for read, errno %d (%s) -- continuing\n", progname, fname, errno, strerror (errno));
        return;
    }
    nbytes = sizeof (int) * optN;
    if ((counters = malloc (nbytes)) == NULL) {
        fprintf (stderr, "%s:  insufficient memory to allocate counters[] (%d bytes, errno %d (%s))\n", progname, nbytes, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    memset (counters, 0, nbytes);
    // we don't check the return value -- if it's smaller, we'll have zeros, which is fine
    fread (counters, nbytes, 1, fp);
    fclose (fp);
    for (i = 0; i < optN; i++) {
        attr_gif [i].count = attr_txt [i].count = counters [i];
    }
}

static void
write_file (char *fname, int counter, int val)
{
    FILE    *fp;

    if ((fp = fopen (fname, "r+")) == NULL) {
        if ((fp = fopen (fname, "w+")) == NULL) {
            fprintf (stderr, "%s:  couldn't open %s for read/write append, errno %d (%s) -- continuing\n", progname, fname, errno, strerror (errno));
            return;
        }
    }
    fseek (fp, counter * sizeof (int), SEEK_SET);
    fwrite (&val, sizeof (val), 1, fp);
    fclose (fp);
}

