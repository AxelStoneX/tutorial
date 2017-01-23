
/*
 *  daq.c
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
 *  This module contains the shared memory region handler and the
 *  data acquisition thread.
 *
 *  2003 06 23  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

#include "adios.h"

#include "../api/api.h"

extern  char                *progname;      // main.c
extern  char                *version;       // version.c

extern  int                 optp;           // main.c, period (ms)
extern  const   char        *opts;          // main.c, shared memory name
extern  int                 optS;           // main.c, number of samples in shared memory
extern  int                 optv;           // main.c
extern  adios_t             *adios;         // adios.c, global information structure about all devices (see adios.h)
extern  int                 nadios;         // adios.c, how many are in the structure above

#define FILLER_ALIGN_4kbytes(n) (4095-(((n)+4095)&4095))
#define FILLER_ALIGN_32bits(n)  (3-(((n)+3)&3))

static  int                 shmem_fd;       // shared memory file descriptor
static  uint8_t             *shmem_ptr;     // pointer to shared memory
static  adios_signature_t   *sig;           // pointer to signature within shared memory
static  adios_daq_status_t  *daq;           // pointer to data acquisition control structure within shared memory
static  adios_cis_t         *cis;           // pointer to one or more card information structures within shared memory
static  uint8_t             *database;      // pointer to base of data within shared memory

void *
daq_thread (void *not_used)
{
    int                 head, tail;
    int                 i;
    int                 sts;
    int                 ai, di;
    dcmd_get_adis_t     *c;
    int                 xfersize;
    void                *ptr;
    adios_data_header_t *dhdr;
    static int          memory_signed = 0;  // flag to indicate if we've signed the data area yet

    // calculate the *maximum* transfer size
    ai = di = 0;
    for (i = 0; i < nadios; i++) {
        if (adios [i].ndi > di) {
            di = adios [i].ndi;
        }
        if (adios [i].nai > ai) {
            ai = adios [i].nai;
        }
    }

    // allocate a buffer which we never free
    xfersize = ai * 2 + di / 8;

    c = malloc (sizeof (*c) + xfersize);
    if (c == NULL) {
        fprintf (stderr, "%s:  couldn't malloc %d bytes, errno %d (%s)\n", progname, sizeof (*c) + xfersize, errno, strerror (errno));
        exit (EXIT_FAILURE);                // trash the process; no real use in continuing without this thread
    }

    // now loop forever, acquiring samples
    while (1) {
        // do one sample

        // adjust the head in a local working copy while we futz with the data
        head = daq -> head + 1;
        if (head >= optS) {
            head = 0;
        }

        // adjust the tail in an atomic manner so that it's always valid
        if (daq -> tail == head) {
            tail = daq -> tail + 1;
            if (tail >= optS) {
                tail = 0;
            }
            daq -> tail = tail;
        }

        // get the data
        ptr = (void *) (database + head * daq -> element_size);
        dhdr = ptr;
        ClockTime (CLOCK_REALTIME, NULL, &dhdr -> t0ns);
        ptr = dhdr + 1;

        for (i = 0; i < nadios; i++) {
            c -> i.nais = adios [i].nai;
            c -> i.ndis = adios [i].ndi;
            xfersize = c -> i.nais * 2 + c -> i.ndis / 8;
            sts = devctl (adios [i].fd, DCMD_GET_ADIS, c, xfersize, NULL);
            if (sts != EOK) {
                fprintf (stderr, "%s:  failed devctl DCMD_GET_ADIS, errno %d (%s)\n", progname, sts, strerror (sts));
                exit (EXIT_FAILURE);
            }
            // just memcpy the data from there to shmem
            memcpy (ptr, c -> o.buf, xfersize);
            ptr = (void *) ((char *) ptr + xfersize);
        }

        ClockTime (CLOCK_REALTIME, NULL, &dhdr -> t1ns);

        // finally, set the daq -> head to our "working" head now that the data is stable
        daq -> head = head;

        /*
         *  See if we need to sign the data area.  We only do this after
         *  at least one sample has been placed into shared memory.
        */

        if (!memory_signed) {
            memcpy (sig -> signature, ADIOS_SIGNATURE, sizeof (sig -> signature));
            memory_signed = 1;
        }

        // wait for the next sample time to occur
        delay (optp);
    }
}

/*
 *  Create the shared memory region.
 *
 *  We create a shared memory region that has the following major components:
 *
 *      o   Control
 *      o   Data
 *
 *  The Control area contains items such as:
 *
 *      o   head, tail, and data information:
 *          -   indicates where data is currently being written
 *          -   indicates time of data write (and validity flag)
 *
 *      o   card information (adios_cis_t):
 *          -   number of AI/AO/DI/DO points per card
 *          -   name of card's driver
 *          -   offset into per-sample information for each card
 *
 *  The Data area contains the samples, prefixed with a small header
 *  (adios_data_header_t):
 *      o   t0ns, t1ns
 *          -   these two snapshots represent the beginning time and
 *              the end time when the sample was taken.
 *
 *  In order to store this information, the shared memory region is created
 *  with the size determined as follows:
 *
 *      size = control + alignment_c + data + alignment_d
 *
 *      alignment_c = sufficient data to fill out a 4k page
 *      alignment_d = sufficient data to fill out a 4k page
 *
 *      control = card_info_structure * number_of_cards + globalinfo
 *      globalinfo = information global to the entire shared memory segment
 *
 *      data = card data in same order as given in card_info_structure with
 *             data elements given in the order analog in, digital in, with
 *             padding for 4 byte boundary.
*/

void
create_shmem (void)
{
    int     size, size_c, size_d, size_d_ai, size_d_di, size_element;
    int     i;
    int     sts;

    size_c = sizeof (adios_signature_t) + sizeof (adios_daq_status_t) + sizeof (adios_cis_t) * nadios;

    size_d = sizeof (adios_data_header_t);
    for (i = 0; i < nadios; i++) {
        size_d_ai = adios [i].nai * ((adios [i].maxresai + 15) / 16) * 2;
        size_d_di = (adios [i].ndi + 31) / 32 * 4;
        size_d += size_d_ai + FILLER_ALIGN_32bits (size_d_ai) + size_d_di;
    }
    size_element = size_d;
    size_d *= optS;

    size = size_c + FILLER_ALIGN_4kbytes (size_c) + size_d + FILLER_ALIGN_4kbytes (size_d);
    
    sts = shm_open (opts, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (sts == -1) {
        if (errno == EEXIST) {          // in the case that it already exists, we'll wipe the signature of the existing one and unlink it.
            printf ("%s:  shared memory region \"%s\" already exists, removing it and restarting\n", progname, opts);
            sts = shm_open (opts, O_RDWR, 0);
            if (sts == -1) {
                fprintf (stderr, "%s:  couldn't open shared memory region \"%s\" in order to erase it (errno %d \"%s\").  Giving up.\n", progname, opts, errno, strerror (errno));
                exit (EXIT_FAILURE);
            }
            shmem_ptr = mmap (0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, sts, 0);
            if (shmem_ptr == MAP_FAILED) {
                fprintf (stderr, "%s:  couldn't mmap shared memory region \"%s\" in order to erase it (errno %d \"%s\").  Giving up.\n", progname, opts, errno, strerror (errno));
                exit (EXIT_FAILURE);
            }
            memset (shmem_ptr, 0, 4096);        // wipe it
            close (sts);
            munmap (shmem_ptr, 4096);
            shm_unlink (opts);
            // now try again to open it!
            sts = shm_open (opts, O_RDWR | O_CREAT | O_EXCL, 0644);
            if (sts == -1) {
                fprintf (stderr, "%s:  couldn't open shared memory region \"%s\" after erasing it (errno %d \"%s\").  Giving up.\n", progname, opts, errno, strerror (errno));
                exit (EXIT_FAILURE);
            }
        } else {
            fprintf (stderr, "%s:  can't create shared memory region \"%s\" -- errno %d (%s)\n", progname, opts, errno, strerror (errno));
            exit (EXIT_FAILURE);                // if the initial open failed for any reason *other than* EEXIST, die.
        }
    }
    shmem_fd = sts;

    sts = ftruncate (shmem_fd, size);
    if (sts == -1) {
        fprintf (stderr, "%s:  can't change size of shared memory region \"%s\" to %d bytes -- errno %d (%s)\n", progname, opts, size, errno, strerror (errno));
        close (shmem_fd);
        shm_unlink (opts);
        exit (EXIT_FAILURE);
    }

    shmem_ptr = mmap (0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);
    if (shmem_ptr == MAP_FAILED) {
        fprintf (stderr, "%s:  can't mmap() shared memory region \"%s\" for %d bytes -- errno %d (%s)\n", progname, opts, size, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    // set up our utility pointers
    sig = (adios_signature_t *) shmem_ptr;                              // signature is at the beginning
    daq = (adios_daq_status_t *) (shmem_ptr + sizeof (*sig));           // data acquisition status is immediately after
    cis = (adios_cis_t *) (shmem_ptr + sizeof (*sig) + sizeof (*daq));  // finally followed by multiple CIS structures

    // now we fill the shared memory structure

    // clear the signature (it's just for safety, a new shmem region is zero'd anyway)
    memset (sig -> signature, 0, sizeof (sig -> signature));
    sig -> datablock = (size_c + FILLER_ALIGN_4kbytes (size_c)) / 4096;
    sig -> num_cis = nadios;
    sig -> num_elems = optS;
    database = shmem_ptr + sig -> datablock * 4096;

    daq -> element_size = size_element;

    // head points to the last entry in the buffer, so that when we add our first entry head will point to 0
    daq -> tail = daq -> head = optS - 1;

    for (i = 0; i < nadios; i++) {
        strncpy (cis [i].name, adios [i].name, MAXNAME);
        cis [i].nai = adios [i].nai;
        cis [i].nao = adios [i].nao;
        cis [i].ndi = adios [i].ndi;
        cis [i].ndo = adios [i].ndo;
        cis [i].nbpc = adios [i].nbpc;
        cis [i].maxresai = adios [i].maxresai;
    }

}

