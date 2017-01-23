
/*
 *  pcl711.c
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
 *  This module contains the PCL-711 functionality.
 *
 *  2003 06 17  R. Krten        created
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

#include "pcl711.h"

extern  char    *progname;                      // main.c
extern  char    *version;                       // version.c

extern  int     opt01;                          // main.c, default for output is 0 or 1?
extern  int     optd;                           // main.c, debug/diag mode
extern  int     optv;                           // main.c

pcl711_t        *pcl711;                        // global information structure about all cards (see pcl711.h)
int             npcl711;                        // how many are in the structure above

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

pcl711_t *
install_card (char *name, int port)
{
    pcl711_t        *p;

    pcl711 = realloc (pcl711, sizeof (pcl711_t) * (npcl711 + 1));
    if (pcl711 == NULL) {
        fprintf (stderr, "%s:  out of memory trying to add port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    p = &pcl711 [npcl711++];

    memset (p, 0, sizeof (*p));

    p -> port = port;
    if (!name) {
        name = strdup ("/dev/pcl711-xxxx");     // create one big enough for the generated name
        if (!name) {
            fprintf (stderr, "%s:  out of memory trying to generate name for port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
        sprintf (name, "/dev/pcl711-%04x", port);   // now put in the real name
        p -> name = name;
    } else {
        p -> name = strdup (name);  // create copy of theirs
    }

    iofunc_attr_init (& p -> attr, S_IFREG | 0666, 0, 0);
    p -> attr.inode = npcl711;                  // inode is index + 1 (inode 0 means "not in use)
    pcl711_write_digital (p, 0);                // initialize it

    /*
     *  Verify that we have a "live" board, or at least a piece
     *  of hardware that looks like a PCL-711 board.  This is
     *  done by trying to read an analog value.
    */

    if (pcl711_read_analog (p, 0) < 0) {        // ooops; not a good board!
        fprintf (stderr, "%s:  unable to read an analog value (port 0x%04X); please check the I/O port address to ensure it is correct (remember that ON=0 and OFF=1 in the address switches)\n", progname, port);
        exit (EXIT_FAILURE);
    }

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
            pcl711_set_gain (p, i, 0);
        }
        while (1)   {
            for (i = 0; i < 8; i++) {
                printf ("[%d] %03X ", i, pcl711_read_analog (p, i));        // no error checking here
            }
            printf ("[DI] %04X ", pcl711_read_digital (p));
            printf ("[AO] %03X ", j);
            pcl711_write_analog (p, j++);
            if (j < 0 || j > 0xfff) {
                j = 0;
            }
            printf ("[DO] %04X ", k);
            pcl711_write_digital (p, k);
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
 *  the PCL-711 card.  While the parser_parse() function does some
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
    pcl711_t            *card;

    // ensure we have something to work on
    if (!data) {
        return (0);     // not an error -- we just didn't have anything in the database
    }
    ncards = 0;

    for (nd = 0; nd < data -> ndevices; nd++) {
        parser_device_t *d;
        int             nc;

        d = &data -> devices [nd];
        if (strncmp (d -> devname, "/dev/pcl711-", strlen ("/dev/pcl711-"))) {
            fprintf (stderr, "%s:  device name \"%s\" is not of the form \"/dev/pcl711-xxxx\" and therefore cannot be configured automatically from configuration file\n", progname, d -> devname);
            exit (EXIT_FAILURE);
        }

        // the card name matches the standard form, install it
        card = install_card (d -> devname, strtol (d -> devname + strlen ("/dev/pcl711-"), NULL, 16));
        ncards++;

        /*
         *  Now verify the configuration and configure the card.
         *  The PCL-711 is a fixed configuration card only, with the 
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
                            fprintf (stderr, "%s:  channel %d (tag %s) is not an analog input channel and the PCL-711 requires that channels 0..7 be analog inputs\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }

                        // verified.  It's an input and we're happy.  Fetch the gain.
                        if (aio -> gain < AIGAINMIN || aio -> gain > AIGAINMAX) {
                            fprintf (stderr, "%s:  channel %d (tag %s) has a gain value (%d) that's out of range {%d..%d}\n", progname, i, aio -> tag ? aio -> tag : "<none>", aio -> gain, AIGAINMIN, AIGAINMAX);
                            exit (EXIT_FAILURE);
                        }

                        // gain verified, configure it.
                        pcl711_set_gain (card, i, aio -> gain);
                    }

                    if (i == 8) {                   // channel 8 better be an output
                        if (!aio -> direction) {    // wants to default, let it
                            aio -> direction = 'o';
                        }
                        if (aio -> direction != 'o') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not an analog output channel and the PCL-711 requires that channel 8 be analog output\n", progname, i, aio -> tag ? aio -> tag : "<none>");
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
                            fprintf (stderr, "%s:  channel %d (tag %s) is not a digital input channel and the PCL-711 requires that channel 9 be a digital input\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }
                    }

                    if (i == 10) {                  // channel 10 better be an output
                        if (!dio -> direction) {    // wants to default, let it
                            dio -> direction = 'o';
                        }
                        if (dio -> direction != 'o') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not a digital output channel and the PCL-711 requires that channel 10 be a digital output\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }
                        pcl711_write_digital (card, opt01 ? 0xffff : 0);
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
 *  pcl711_read_analog (pcl711, channel)
 *
 *  Reads the analog value from the analog to digital convertor.
 *  If the DRDY bit is set to one, we wait -- this is because the conversion
 *  takes a maximum of 25 microseconds (in practice, on a Pentium 2 @ 350 MHz,
 *  there were between 13 and 15 iterations of the do/while loop).
 *
 *  The gain used for the channel is stored in the pcl711_t structure and
 *  is fetched from there.
 *
 *  The value 0x000 -> 0xFFF is returned.
*/

int
pcl711_read_analog (pcl711_t *pcl, int channel)
{
    int     data;
    int     base;
    int     timeout;
    static int calibrated = 0;
    static struct timespec when;

    if (!calibrated) {
        nanospin_calibrate (1);                 // perform with interrupts off
        nsec2timespec (&when, PCL711_DELAY);    // set up delay value
        calibrated = 1;
    }

    channel &= 7;                               // for inrange
    base = pcl -> port;

    out8 (base + PCL711_MUX_SCAN_CONTROL, channel);

    out8 (base + PCL711_GAIN_CONTROL, pcl -> gains [channel]);

    out8 (base + PCL711_SOFTWARE_AD_TRIGGER, 0 /* any data */);

    timeout = PCL711_TIMEOUT;
    do {
        data = in8 (base + PCL711_ANALOG_HIGH);
        nanospin (&when);
    } while ((data & PCL711_ANALOG_HIGH_DRDY) && (timeout-- >= 0));

    if (timeout < 0) {
        return (-1);
    }
    data = ((data & 0x0f) << 8) + in8 (base + PCL711_ANALOG_LOW);
    return (data);
}

void
pcl711_write_analog (pcl711_t *pcl, int value)
{
    out8 (pcl -> port + PCL711_ANALOG_LOW, value & 0xff);
    out8 (pcl -> port + PCL711_ANALOG_HIGH, (value & 0xf00) >> 8);
}

void
pcl711_set_gain (pcl711_t *pcl, int channel, int gaincode)
{
    if (gaincode < 0 || gaincode > 4) {
        return;
    }
    channel &= 7;
    pcl -> gains [channel] = gaincode;
}

int
pcl711_get_gain (pcl711_t *pcl, int channel)
{
    channel &= 7;
    return (pcl -> gains [channel]);
}

int
pcl711_read_digital (pcl711_t *pcl)
{
    return (in8 (pcl -> port + PCL711_DI_LOW) + 256 * in8 (pcl -> port + PCL711_DI_HIGH));
}

void
pcl711_write_digital (pcl711_t *pcl, int value)
{
    out8 (pcl -> port + PCL711_DO_LOW, value & 0xff);
    out8 (pcl -> port + PCL711_DO_HIGH, value >> 8);
    pcl -> dout = value;
}

static int bits [16] = {
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000, 0x2000, 0x4000, 0x8000
};

void
pcl711_write_digital_bit (pcl711_t *pcl, int bitnum, int bitval)
{
    bitnum &= 15;           // guarantee range

    if (bitval) {
        pcl -> dout |= bits [bitnum];
    } else {
        pcl -> dout &= ~bits [bitnum];
    }

    if (bitnum < 8) {       // 0 .. 7 are in the first byte
        out8 (pcl -> port + PCL711_DO_LOW, pcl -> dout & 0xff);
    } else {
        out8 (pcl -> port + PCL711_DO_HIGH, pcl -> dout >> 8);
    }
}

int
pcl711_read_digital_bit (pcl711_t *pcl, int bitnum)
{
    bitnum &= 15;           // guarantee range

    if (bitnum < 8) {
        return (!!(in8 (pcl -> port + PCL711_DI_LOW) & bits [bitnum]));
    } else {
        return (!!(in8 (pcl -> port + PCL711_DI_HIGH) & bits [bitnum - 8]));
    }
}

