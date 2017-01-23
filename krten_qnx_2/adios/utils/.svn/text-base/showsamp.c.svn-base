
/*
 *  showsamp.c
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
 *  This module represents the main module for the "Show Sample" utility,
 *  which dumps out samples out of the shared memory region maintained
 *  by ADIOS.
 *
 *  It also serves as an example of how to retrieve values from the shared
 *  memory region.
 *
 *  2003 06 24  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

#include "api.h"

static  void                optproc (int, char **);
static  void                do_data_set (int i);

const   char                *progname = "showsamp";
const   char                *blankname= "        ";
extern  char                *version;       // showsamp_version.c

static  int                 optn;
static  const   char        *opts;
static  int                 optv;

// this set of variables deals with the shared memory
static  int                 shmem_fd;       // file descriptor
static  uint8_t             *shmem_ptr;     // pointer to shared memory
static  int                 shmem_size;     // number of bytes in shared memory region
static  adios_signature_t   *sig;           // pointer to signature within shared memory
static  adios_daq_status_t  *daq;           // pointer to data acquisition control structure within shared memory
static  adios_cis_t         *cis;           // pointer to one or more card information structures within shared memory
static  uint8_t             *database;      // pointer to base of data within shared memory

int
main (int argc, char **argv)
{
    struct  stat    statbuf;
    int             i;
    int             head;
    char            buf [BUFSIZ];

    optproc (argc, argv);

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

    // print out some information
    printf ("\nSHOWSAMP\n");
    printf ("Shared memory region:  %s\n", opts);
    printf ("Size:                  %d bytes\n", shmem_size);
    printf ("Devices: (%d)\n", sig -> num_cis);
    for (i = 0; i < sig -> num_cis; i++) {
        printf ("\t[%d] \"%s\" %d x AI, %d x DI\n", i, cis [i].name, cis [i].nai, cis [i].ndi);
    }

    // retrieve common information
    database = shmem_ptr + sig -> datablock * 4096;

    // retrieve "optn" data sets
    head = daq -> head;
    printf ("HEAD %d TAIL %d ELEMENT_SIZE %d NUM_ELEMS %d\n", head, daq -> tail, daq -> element_size, sig -> num_elems);
    for (i = head - optn + 1; i <= head; i++) {
        if (i < 0) {
            do_data_set (i + sig -> num_elems);
        } else {
            do_data_set (i);
        }
    }

    return (EXIT_SUCCESS);
}

static void
do_data_set (int ds)
{
    int                     i, j;
    adios_data_header_t     *dhead;
    uint16_t                *u16;
    struct tm               *tm;
    char                    buf [BUFSIZ];
        time_t num_secs;

    dhead = (adios_data_header_t *) (database + ds * daq -> element_size);
    num_secs = dhead -> t0ns / 1000000000LL;
    tm = localtime (&num_secs);
    strftime (buf, sizeof (buf), "%Y %m %d %H:%M:%S", tm);

    printf ("Data Set %d, acquired %s.%09lld (delta %lld nanoseconds)\n", ds, buf, dhead -> t1ns % 1000000000LL, dhead -> t1ns - dhead -> t0ns);
    u16 = (uint16_t *) (dhead + 1);
    for (i = 0; i < sig -> num_cis; i++) {
        printf ("CARD %s, %d AI, (%d AO), %d DI, (%d DO)\n", cis [i].name, cis [i].nai, cis [i].nao, cis [i].ndi, cis [i].ndo);
        if (cis [i].nai) {
            printf ("    AI: ");
            for (j = 0; j < cis [i].nai; j++) {
                printf ("%04X ", *u16++);
            }
            printf ("\n");
        }
        if (cis [i].ndi) {
            printf ("    DI: ");
            for (j = 0; j < (cis [i].ndi + 15) / 16; j++) {
                printf ("%04X ", *u16++);
            }
            printf ("\n");
        }
    }
    printf ("\n");
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

    optn = 5;
    opts = "/adios";

    while ((opt = getopt (argc, argv, "n:s:v")) != -1) {
        switch (opt) {
        case    'n':
            optn = atoi (optarg);
            if (optn < 1) {
                fprintf (stderr, "%s:  number of samples (-n option) must be > 0\n", progname);
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

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }
}

