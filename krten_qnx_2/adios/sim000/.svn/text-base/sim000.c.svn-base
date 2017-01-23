
/*
 *  sim000.c
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
 *  This module contains the SIM-000 functionality.
 *
 *  2003 10 09  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <hw/inout.h>

#include "sim000.h"

extern  char    *progname;                      // main.c
extern  char    *version;                       // version.c

extern  int     opt01;                          // main.c, default for output is 0 or 1?
extern  int     optd;                           // main.c, debug/diag mode
extern  int     optv;                           // main.c

sim000_t        *sim000;                        // global information structure about all cards (see sim000.h)
int             nsim000;                        // how many are in the structure above

/*
 *  install_card
 *
 *  This is the low-level function that adds (and verifies) a card to
 *  the driver.  It takes the port name and the port address.
 *
 *  Verification is accomplished by reading the value from the analog port,
 *  because the analog port needs to be polled, we can determine if the card
 *  is present.
*/

sim000_t *
install_card (char *name, int port)
{
    sim000_t        *p;

    sim000 = realloc (sim000, sizeof (sim000_t) * (nsim000 + 1));
    if (sim000 == NULL) {
        fprintf (stderr, "%s:  out of memory trying to add port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    p = &sim000 [nsim000++];

    memset (p, 0, sizeof (*p));

    p -> port = port;
    if (!name) {
        name = strdup ("/dev/sim000-xxxx");     // create one big enough for the generated name
        if (!name) {
            fprintf (stderr, "%s:  out of memory trying to generate name for port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
        sprintf (name, "/dev/sim000-%04x", port);   // now put in the real name
        p -> name = name;
    } else {
        p -> name = strdup (name);  // create copy of theirs
    }

    iofunc_attr_init (& p -> attr, S_IFREG | 0666, 0, 0);
    p -> attr.inode = nsim000;                  // inode is index + 1 (inode 0 means "not in use)
    sim000_write_digital (p, 0);                // initialize it

    /*
     *  DEBUG/DIAGNOSTICS
     *
     *  If -d is given on the command line, then this program will *not*
     *  become a resource manager, but will instead continually dump out
     *  its data that it reads, once per second.
    */

    if (optd) {
        int i, j, k;
    
        for (i = 0; i < NAI; i++) {
            sim000_set_gain (p, i, 0);
        }
        while (1)   {
            for (i = 0; i < 8; i++) {
                printf ("[%d] %03X ", i, sim000_read_analog (p, i));        // no error checking here
            }
            printf ("[DI] %04X ", sim000_read_digital (p));
            printf ("[AO] %03X ", j);
            sim000_write_analog (p, j++);
            if (j < 0 || j > 0xfff) {
                j = 0;
            }
            printf ("[DO] %04X ", k);
            sim000_write_digital (p, k);
            k <<= 1;
            if (!k || k > 0xffff) {
                k = 1;
            }
            printf ("\n");
            sleep (1);
        }
    }

    return (p);
}

/*
 *  install_cards_from_database (data)
 *
 *  This function takes the parser_t output of the parser_parse()
 *  function and validates that the settings are appropriate for
 *  the SIM-000 simulator.  While the parser_parse() function does some
 *  basic validation, it doesn't know enough about the specific
 *  card to perform full validation.  The ports are installed
 *  (via "install_card()" above) validated, and configured.
 *
 *  This function either exit()s or returns the number of cards
 *  successfully installed and configured.
*/

int
install_cards_from_database (parser_t *data)
{
    int                 nd;
    int                 ncards;
    int                 i;
    sim000_t            *card;

    // ensure we have something to work on
    if (!data) {
        return (0);     // not an error -- we just didn't have anything in the database
    }
    ncards = 0;

    for (nd = 0; nd < data -> ndevices; nd++) {
        parser_device_t *d;
        int             nc;

        d = &data -> devices [nd];
        if (strncmp (d -> devname, "/dev/sim000-", strlen ("/dev/sim000-"))) {
            fprintf (stderr, "%s:  device name \"%s\" is not of the form \"/dev/sim000-xxxx\" and therefore cannot be configured automatically from configuration file\n", progname, d -> devname);
            exit (EXIT_FAILURE);
        }

        // the card name matches the standard form, install it
        card = install_card (d -> devname, strtol (d -> devname + strlen ("/dev/sim000-"), NULL, 16));
        ncards++;

        /*
         *  Now verify the configuration and configure the card.
         *  The SIM-000 is a fixed configuration card only (and looks identical to a PCL-711), with the 
         *  channels set up as follows:
         *
         *      Channels 0 through 7 are always AIN
         *      Channel 8 is always AOUT
         *      Channel 9 is the DIN with 16 bits
         *      Channel 10 is the DOUT with 16 bits
         *      No other channels are supported.
         *
         *  Remember:  ADIOS is not just a cute acronym, but also reflects the
         *  order of channels:  primary order A, D; secondary order I, O; giving
         *  the final order AI/AO/DI/DO.
        */

        for (nc = 0; nc < d -> nchannels; nc++) {
            parser_channel_t    *c;
            int                 np;

            c = &d -> channels [nc];
            for (np = 0; np < c -> nports; np++) {
                parser_port_t   *p;
                int             naio;
                int             ndio;

                p = &c -> ports [np];

                // check all analog channels
                for (naio = 0; naio < p -> naios; naio++) {
                    parser_aio_t *aio;

                    aio = &p -> aios [naio];

                    // we have an analog channel, verify 0-7 are AIN and 8 is AOUT
                    i = atoi (c -> chan);
                    if (i < 0 || i > 8) {
                        fprintf (stderr, "%s:  channel %d (tag %s) is out of range; should be 0..8\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                        exit (EXIT_FAILURE);
                    }

                    if (0 <= i && i <= 7) {         // channels 0 through 7 better be inputs
                        if (!aio -> direction) {    // wants to default, let it
                            aio -> direction = 'i';
                        }
                        if (aio -> direction != 'i') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not an analog input channel and the SIM-000 requires that channels 0..7 be analog inputs\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }

                        // verified.  It's an input and we're happy.  Fetch the gain.
                        if (aio -> gain < AIGAINMIN || aio -> gain > AIGAINMAX) {
                            fprintf (stderr, "%s:  channel %d (tag %s) has a gain value (%d) that's out of range {%d..%d}\n", progname, i, aio -> tag ? aio -> tag : "<none>", aio -> gain, AIGAINMIN, AIGAINMAX);
                            exit (EXIT_FAILURE);
                        }

                        // gain verified, configure it.
                        sim000_set_gain (card, i, aio -> gain);
                    }

                    if (i == 8) {                   // channel 8 better be an output
                        if (!aio -> direction) {    // wants to default, let it
                            aio -> direction = 'o';
                        }
                        if (aio -> direction != 'o') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not an analog output channel and the SIM-000 requires that channel 8 be analog output\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }
                    }
                }

                // check all digital channels
                for (ndio = 0; ndio < p -> ndios; ndio++) {
                    parser_dio_t *dio;

                    dio = &p -> dios [ndio];

                    // we have a digital channel, verify channel 9 is DIN and channel 10 is DOUT
                    i = atoi (c -> chan);
                    if (i < 9 || i > 10) {
                        fprintf (stderr, "%s:  channel %d (tag %s) is out of range; should be 9 or 10\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                        exit (EXIT_FAILURE);
                    }

                    if (i == 9) {                   // channel 9 better be an input
                        if (!dio -> direction) {    // wants to default, let it
                            dio -> direction = 'i';
                        }
                        if (dio -> direction != 'i') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not a digital input channel and the SIM-000 requires that channel 9 be a digital input\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }
                    }

                    if (i == 10) {                  // channel 10 better be an output
                        if (!dio -> direction) {    // wants to default, let it
                            dio -> direction = 'o';
                        }
                        if (dio -> direction != 'o') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not a digital output channel and the SIM-000 requires that channel 10 be a digital output\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }
                        sim000_write_digital (card, opt01 ? 0xffff : 0);
                    }

                    // now that the channels are correct, verify the bit number as being in range 0..15
                    if (dio -> bitnum < 0 || dio -> bitnum > 15) {
                        fprintf (stderr, "%s:  channel %d (tag %s) bit number (%d) is not in range {0..15}\n", progname, i, dio -> tag ? dio -> tag : "<none>", dio -> bitnum);
                        exit (EXIT_FAILURE);
                    }
                }
            }
        }
    }
    return (ncards);
}

/*
 *  sim000_read_analog (sim000, channel)
 *
 *  Reads the analog value from the analog to digital convertor.
 *  If the DRDY bit is set to one, we wait -- this is because the conversion
 *  takes a maximum of 25 microseconds (in practice, on a Pentium 2 @ 350 MHz,
 *  there were between 13 and 15 iterations of the do/while loop).
 *
 *  The gain used for the channel is stored in the sim000_t structure and
 *  is fetched from there.
 *
 *  The value 0x000 -> 0xFFF is returned.
*/

int
sim000_read_analog (sim000_t *sim, int channel)
{
    static int value = 0;

    value++;
    if (value > 0xfff) {
        value = 0;
    }
    return (value);
}

void
sim000_write_analog (sim000_t *sim, int value)
{
    // done
}

void
sim000_set_gain (sim000_t *sim, int channel, int gaincode)
{
    if (gaincode < 0 || gaincode > 4) {
        return;
    }
    channel &= 7;
    sim -> gains [channel] = gaincode;
}

int
sim000_get_gain (sim000_t *sim, int channel)
{
    channel &= 7;
    return (sim -> gains [channel]);
}

int
sim000_read_digital (sim000_t *sim)
{
    static int value = 0;

    value++;
    if (value > 0xffff) {
        value = 0;
    }
    return (value);
}

void
sim000_write_digital (sim000_t *sim, int value)
{
}

void
sim000_write_digital_bit (sim000_t *sim, int bitnum, int bitval)
{
}

int
sim000_read_digital_bit (sim000_t *sim, int bitnum)
{
    static int value = 0;

    value = !value;
    return (value);
}

