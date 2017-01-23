
/*
 *  tag.c
 *
 *  main.c shell version 0.005
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
 *  This module represents the main module for the tag display utility.
 *
 *  This program will display, monitor, and set tags from the ADIOS
 *  database.
 *
 *  2003 07 10  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <ctype.h>

#include "api.h"

#include "parser.h"
#include "client.h"

#include "tag.h"

static  void    optproc (int, char **);
static  int     tag_operation (char *p);
static  void    process_set (char *p, int type);
static  void    add_to_display_list (char *p);
static  void    fixup_display_list (void);
static  void    display_list (void);
static  void    setup_sharedmem (void);

static  const   char        *progname = "tag";
extern  char                *version;       // tag_version.c

static  char                *optc;          // configuration file
static  int                 optm;           // millisecond delay if monitoring (-1 == off)
static  const char          *opts;          // shared memory name
static  int                 optv;

static  parser_t            *parse_tree;    // the parse tree (for everything in the config file)

// this set of variables deals with the shared memory
static  int                 shmem_fd;       // file descriptor
static  uint8_t             *shmem_ptr;     // pointer to shared memory
static  int                 shmem_size;     // number of bytes in shared memory region
static  adios_signature_t   *sig;           // pointer to signature within shared memory
static  adios_daq_status_t  *daq;           // pointer to data acquisition control structure within shared memory
static  adios_cis_t         *cis;           // pointer to one or more card information structures within shared memory
static  uint8_t             *database;      // pointer to base of data within shared memory

// this is the database of things to display
static  int                 ntags;
static  tag_t               *tags;

int
main (int argc, char **argv)
{
    optproc (argc, argv);

    setup_sharedmem ();

    fixup_display_list ();                  // this gets the rest of the information for the display list entries into the database

    do {
        display_list ();
        if (optm != -1) {
            delay (optm);
        }
    } while (optm != -1);

    return (EXIT_SUCCESS);
}

static void
setup_sharedmem (void)
{
    char    buf [BUFSIZ];
    struct  stat    statbuf;

    // open the shared memory
    sprintf (buf, "/dev/shmem/%s", opts);
    if (stat (buf, &statbuf) == -1) {
        fprintf (stderr, "%s:  can't stat \"%s\", errno %d (%s)\n", progname, buf, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    if ((shmem_fd = shm_open (opts, O_RDONLY, 0)) == -1) {
        fprintf (stderr, "%s:  can't open \"%s\" for O_RDONLY, errno %d (%s)\n", progname, opts, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    shmem_size = statbuf.st_size;

    // map the shared memory
    shmem_ptr = mmap (0, shmem_size, PROT_READ, MAP_SHARED, shmem_fd, 0);
    if (shmem_ptr == MAP_FAILED) {
        fprintf (stderr, "%s:  can't mmap() shared memory region \"%s\" for %d bytes -- errno %d (%s)\n", progname, opts, shmem_size, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    // set up our utility pointers
    sig = (adios_signature_t *) shmem_ptr;                              // signature is at the beginning
    daq = (adios_daq_status_t *) (shmem_ptr + sizeof (*sig));           // data acquisition status is immediately after
    cis = (adios_cis_t *) (shmem_ptr + sizeof (*sig) + sizeof (*daq));  // finally followed by multiple CIS structures

    if (memcmp (sig -> signature, ADIOS_SIGNATURE, sizeof (sig -> signature))) {
        fprintf (stderr, "%s:  signature [%02X %02X %02X %02x] does not match valid ADIOS signature, aborting!\n", progname, sig -> signature [0], sig -> signature [1], sig -> signature [2], sig -> signature [3]);
        exit (EXIT_FAILURE);
    }

    // retrieve common information
    database = shmem_ptr + sig -> datablock * 4096;
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
    parser_handle_t     *parse_handle;

    if (!argc) {
        usageError ();
    }

    optc = NULL;
    optm = -1;
    opts = "/adios";

    while ((opt = getopt (argc, argv, "c:m:s:v")) != -1) {
        switch (opt) {
        case    'c':
            optc = optarg;
            break;
        case    'm':
            optm = atoi (optarg);
            if (optm < 1) {
                fprintf (stderr, "%s:  -m value must be positive and not zero\n", progname);
                exit (EXIT_FAILURE);
            }
            break;
        case    's':
            opts = optarg;
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

    if (parse_handle = parser_open (optc)) {
        if (parse_tree = parser_parse (parse_handle, "*")) {
            if (parse_tree -> status != PARSER_OK) {
                fprintf (stderr, "%s:  configuration file (%s) error (%s), aborting\n", progname, parse_handle -> fname, parser_strerror (parse_tree -> status));
                exit (EXIT_FAILURE);
            }
            if (optv) {
                parser_dump (parse_tree);
            }
        }
        parser_close (parse_handle);
    } else {
        if (optc) {
            printf ("Didn't find configuration file \"%s\"; aborting\n", optc);
        } else {
            printf ("Didn't find the default configuration file (neither ./adios.cfg nor /etc/adios.cfg)\n");
        }
        exit (EXIT_FAILURE);
    }

    for (; optind < argc; optind++) {
        switch (tag_operation (argv [optind])) {
        case    0:
            add_to_display_list (argv [optind]);
            break;
        case    1:
            process_set (argv [optind], 'c');       // c == converted (use span, raw for digital)
            break;
        case    2:
            process_set (argv [optind], 'r');       // r = raw (don't convert)
            break;
        }
    }
}

/*
 *  tag_operation
 *
 *  This utility looks at the command and determines if it's a plain
 *  tag (in which case 0 is returned), if it's a tag that has one
 *  equals sign, meaning that the "span" value should be used (in
 *  which case 1 is returned) or if there are two equals signs,
 *  meaning that the raw value should be used (in which case 2 is
 *  returned).
*/

static int
tag_operation (char *p)
{
    while (*p) {
        if (*p == '=') {
            if (*++p == '=') {
                return (2);
            }
            return (1);
        }
        p++;
    }
    return (0);
}

static void
process_set (char *p, int rc)
{
    char            *v;     // value
    int             sts;    // status of operation

    if (!strtok (p, "=") || !(v = strtok (NULL, "="))) {
        fprintf (stderr, "%s:  internal error parsing set string, tag \"%s\"\n", progname, p);
        exit (EXIT_FAILURE);
    }

    if (optv) {
        printf ("SETTING %s to %s\n", p, v);
    }

    if (rc == 'r') {
        sts = adios_set_tag_raw (parse_tree, p, atof (v));
    } else {
        sts = adios_set_tag_span (parse_tree, p, atof (v));
    }

    if (sts) {
        fprintf (stderr, "%s:  error setting tag %s to %s (in %s mode), errno %d (%s)\n", progname, p, v, rc == 'c' ? "converted" : "raw", sts, strerror (sts));
        exit (EXIT_FAILURE);
    } else if (optv) {
        printf ("SETTING %s to %s CONFIRMED\n", p, v);
    }
}

static void
add_to_display_list (char *p)
{
    tag_t           *t;

    // This would be the place to add wildcard expansion through the configuration file for tags.  You need to allocate the space for the
    // tag expansion result before you hit fixup_display_list() (otherwise you'll need to move the entries to make room, or add them at the
    // end, etc.)  Also, you'll either need to walk the parse_tree database yourself, or write a routine, 'cuz parser_find_by_tag() doesn't
    // know how to stop and resume searching a wildcard expansion.

    tags = realloc (tags, sizeof (tag_t) * (ntags + 1));
    if (!tags) {
        fprintf (stderr, "%s:  out of memory allocating %d bytes for tags database, errno %d (%s)\n", progname, sizeof (tag_t) * (ntags + 1), errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    t = &tags [ntags++];
    memset (t, 0, sizeof (*t));

    t -> name = strdup (p);
}

/*
 *  fixup_display_list
 *
 *  This takes the tag names in the display list and fetches
 *  the extra information from the shared memory and tags
 *  databases.
 *
 *  We can't do this at "add_to_display_list" time because
 *  we need access to the shared memory, and we don't get
 *  that until we've processed all the command line options.
*/

static void
fixup_display_list (void)
{
    tag_t           *t;
    parser_aio_t    *a;
    parser_dio_t    *d;
    char            *dname;
    char            *p;
    void            *tmp;
    int             type;
    int             chan;
    int             port;
    int             i;
    int             offsetsum;

    /*
     *  go through all tags in our database and fix them
    */

    for (t = tags; t < &tags [ntags]; t++) {

        p = t -> name;

        // check tag validity
        if (!(tmp = parser_find_by_tag (parse_tree, p, &type))) {
            fprintf (stderr, "%s:  tag \"%s\" doesn't exist in the tags database!\n", progname, p);
            exit (EXIT_FAILURE);
        }

        // establish a pointer to the tag database entry in the parser table
        if (type == 'd') {
            d = (parser_dio_t *) tmp;
            a = NULL;
            dname = d -> port -> channel -> device -> devname;
            chan = atoi (d -> port -> channel -> chan);
        } else if (type == 'a') {
            a = (parser_aio_t *) tmp;
            d = NULL;
            dname = a -> port -> channel -> device -> devname;
            chan = atoi (a -> port -> channel -> chan);
        } else {
            fprintf (stderr, "%s:  internal error, tag \"%s\" is neither analog nor digital but is instead of type 0x%02X (%c)\n", progname, p, type, type);
            exit (EXIT_FAILURE);
        }
        t -> type = type;

        // reset the cumulative offset
        offsetsum = 0;

        // see if the device that the tag references exists
        for (i = 0; i < sig -> num_cis; i++) {
            if (!strcmp (dname, cis [i].name)) {
                break;
            }
            // accumulate the cumulative offset (for all cards except ours, thus after the "break")
            offsetsum += cis [i].nai * 2 + cis [i].ndi / 8;     // ASSUMPTION: all analog inputs are 2 bytes.
        }
        if (i == sig -> num_cis) {
            fprintf (stderr, "%s:  can't find device \"%s\" needed by tag \"%s\" in the shared memory database\n", progname, dname, p);
            exit (EXIT_FAILURE);
        }

        /*
         *  ADIOS -- AI/AO/DI/DO ordering.
         *  Our input point must be in the AI or DI space, and must match the type
         *  specified in the tag database.
        */

        if (a) {                // ours is an analog point; it should therefore be within the AI range
            // verify channel
            if (chan > cis [i].nai) {
                fprintf (stderr, "%s:  for tag \"%s\", the tags database says that our point (channel %d) is an analog input point, but the shared memory database disagrees, stating that it only has %d analog inputs\n", progname, p, chan, cis [i].nai);
                exit (EXIT_FAILURE);
            }
            // calculate the offset from the beginning of the dataset corresponding to this card
            t -> offset = chan * 2;     // ASSUMPTION: all analog inputs are 2 bytes
            t -> span_low = a -> span_low;
            t -> span_high = a -> span_high;
        } else {
            // verify channel
            if (chan < cis [i].nai + cis [i].nao || chan > cis [i].nai + cis [i].nao + cis [i].ndi) {
                fprintf (stderr, "%s:  for tag \"%s\", the tags database says that our point (channel %d) is a digital input point, but the shared memory database disagrees, stating that its digital input range is from %d to %d\n", progname, p, chan, cis [i].nai + cis [i].nao, cis [i].nai + cis [i].nao + cis [i].ndi);
                exit (EXIT_FAILURE);
            }

            // verify optional port (port can be '\0', or a lowercase letter starting at 'a' (implying offset of 0)), and fixup offset if any
            port = 0;
            if (isalpha (d -> port -> port [0])) {
                port = tolower (d -> port -> port [0]) - 'a';
            }
            // calculate the offset from the beginning of the dataset corresponding to this card
            t -> offset = cis [i].nai * 2 + (chan - cis [i].nai - cis [i].nao) * cis [i].nbpc + port;       // ASSUMPTION: all analog inputs are 2 bytes, and that all digital ports are 8 bits.
            t -> bitnum = d -> bitnum;
        }

        // add all of the previous cards' offsets
        t -> offset += offsetsum;

        if (optv) {
            printf ("Added tag \"%s\" to display list, type %c offset %d (0x%x) span_low %g span_high %g bitnum %d\n", p, type, t -> offset, t -> offset, t -> span_low, t -> span_high, t -> bitnum);
        }
    }
}

static int bits [16] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000};

static void
display_list (void)
{
    adios_data_header_t     *dhead;
    int                     ds;
    int                     i;
    char                    buf [BUFSIZ];
    uint16_t                *u16;
    uint8_t                 *u8;
    struct tm               *tm;
    double                  v;
        time_t  num_secs;

    ds = daq -> head;
    dhead = (adios_data_header_t *) (database + ds * daq -> element_size);

    num_secs = dhead -> t0ns / 1000000000LL;
    tm = localtime (&num_secs);
    strftime (buf, sizeof (buf), "%Y %m %d %H:%M:%S", tm);
    u16 = (uint16_t *) (dhead + 1);     // get pointer to this sample set
    u8 = (uint8_t *) u16;               // and get an 8-bit version too

    printf ("Data Set %d, acquired %s.%09lld (delta %lld nanoseconds)\n", ds, buf, dhead -> t1ns % 1000000000LL, dhead -> t1ns - dhead -> t0ns);

    for (i = 0; i < ntags; i++) {
        if (tags [i].type == 'a') {
            printf ("Tag \"%s\" raw value 0x%04X", tags [i].name, u16 [tags [i].offset / 2]);
            if (tags [i].span_low != tags [i].span_high) {
                v = u16 [tags [i].offset / 2];
                v = v / 4095. * (tags [i].span_high - tags [i].span_low) + tags [i].span_low;
                printf (" span-compensated value %s%g", v > 0.0 ? "+" : "", v);
            }
            printf ("\n");
        }
        if (tags [i].type == 'd') {
            printf ("Tag \"%s\" %d\n", tags [i].name, !!(u8 [tags [i].offset] & bits [tags [i].bitnum]));
        }
    }
    printf ("\n");
}

