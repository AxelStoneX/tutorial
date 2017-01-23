
/*
 *  dio144.c
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
 *  This module contains the DIO-144 functionality.
 *
 *  2003 07 03  R. Krten        created
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
#include <ctype.h>

#include "dio144.h"

extern  char    *progname;                      // main.c
extern  char    *version;                       // version.c

extern  int     opt01;                          // main.c, initialized state of digital outputs
extern  int     optd;                           // main.c, debug/diag mode
extern  int     optv;                           // main.c

dio144_t        *dio144;                        // global information structure about all cards (see dio144.h)
int             ndio144;                        // how many are in the structure above

dio144_t *
install_card (char *name, int port)
{
    dio144_t        *dio;
    int             i;

    dio144 = realloc (dio144, sizeof (dio144_t) * (ndio144 + 1));
    if (dio144 == NULL) {
        fprintf (stderr, "%s:  out of memory trying to add port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    dio = &dio144 [ndio144++];

    memset (dio, 0, sizeof (*dio));

    dio -> port = port;
    if (!name) {
        name = strdup ("/dev/dio144-xxxx");     // create one big enough for the generated name
        if (!name) {
            fprintf (stderr, "%s:  out of memory trying to generate name for port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
        sprintf (name, "/dev/dio144-%04x", port);   // now put in the real name
        dio -> name = name;
    } else {
        dio -> name = strdup (name);    // create copy of theirs
    }

    iofunc_attr_init (&dio -> attr, S_IFREG | 0666, 0, 0);
    dio -> attr.inode = ndio144;                    // inode is index + 1 (inode 0 means "not in use)

    /*
     *  initialize all channels.
     *
     *  Note that on powerup the DIO-144 card comes up as all inputs anyway;
     *  this is mainly to initialize the software shadow structures (the
     *  "configs" member).
    */

    for (i = 0; i < NCHANS; i++) {
        dio144_write_channel (dio, i, 0);
        dio144_configure_channel_port (dio, i, 0, DIO144_INPUT);
        dio144_configure_channel_port (dio, i, 1, DIO144_INPUT);
        dio144_configure_channel_port (dio, i, 2, DIO144_INPUT);
    }

    /*
     *  DEBUG/DIAGNOSTICS
     *
     *  If -d is given on the command line, then this program will *not*
     *  become a resource manager, but will instead continually dump out
     *  its data that it reads, once per second.
    */

    if (optd) {
        int c, p, k;

        printf ("\n\n");
        printf ("ASSUMPTION!  This diagnostic will read channels 0 through 4, and write\n");
        printf ("channel 5. This diagnostic ASSUMES that channel 0 and channel 5 are shorted\n");
        printf ("together with a 50-pin straight through cable.  If this is not the case,\n");
        printf ("there is no need to worry, except that the outputs sent to channel 5 will\n");
        printf ("not show up in channel zero.\n");
        printf ("\n\n");
        sleep (1);

        // configure the card the way *we* want it for the diags -- all inputs, except chan 5 which is all outputs

        for (k = 0; k < 5; k++) {
            dio144_configure_channel_port (dio, k, 0, DIO144_INPUT);
            dio144_configure_channel_port (dio, k, 1, DIO144_INPUT);
            dio144_configure_channel_port (dio, k, 2, DIO144_INPUT);
        }
        dio144_configure_channel_port (dio, 5, 0, DIO144_OUTPUT);
        dio144_configure_channel_port (dio, 5, 1, DIO144_OUTPUT);
        dio144_configure_channel_port (dio, 5, 2, DIO144_OUTPUT);

        k = 1;
        while (1)   {
            for (c = 0; c < 5; c++) {
                printf ("DI: [C%d] ", c);
                for (p = 0; p < 3; p++) {
                    printf ("P%c=%02X ", p + 'A', dio144_read_channel_port (dio, c, p));
                }
                printf ("\n");
            }
            printf ("DO: [C5] PA=%02X PB=%02X PC=%02X ", k & 0xff, (k & 0xff00) >> 8, (k & 0xff0000) >> 16);
            dio144_write_channel_port (dio, 5, 0, k & 0xff);
            dio144_write_channel_port (dio, 5, 1, (k & 0xff00) >> 8);
            dio144_write_channel_port (dio, 5, 2, (k & 0xff0000) >> 16);
            k <<= 1;
            if (k >= 0x01000000) {
                k = 1;
            }
            printf ("\n\n");

            sleep (1);
        }
    }
    return (dio);
}

/*
 *  install_cards_from_database (data)
 *
 *  This function takes the parser_t output of the parser_parse()
 *  function and validates that the settings are appropriate for
 *  the DIO-144 card.  While the parser_parse() function does some
 *  basic validation, it doesn't know enough about the specific
 *  card to perform full validation.  The ports are installed
 *  (via "install_card()" above) validated, and configured.
 *
 *  This function either exit()s or returns the number of cards
 *  successfully installed and configured.
*/

static const char *portnames [4] = {"A", "B", "C(low)", "C(high)"};

int
install_cards_from_database (parser_t *data)
{
    int                 nd;
    int                 ncards;
    int                 i;
    dio144_t            *card;
    int                 config [6][4];      // 6 for the channels, 4 for parts A, B, Clow, Chigh

    // ensure we have something to work on
    if (!data) {
        return (0);     // not an error -- we just didn't have anything in the database
    }
    ncards = 0;

    for (nd = 0; nd < data -> ndevices; nd++) {
        parser_device_t *d;
        int             nc;

        d = &data -> devices [nd];
        if (strncmp (d -> devname, "/dev/dio144-", strlen ("/dev/dio144-"))) {
            fprintf (stderr, "%s:  device name \"%s\" is not of the form \"/dev/dio144-xxxx\" and therefore cannot be configured automatically from configuration file\n", progname, d -> devname);
            exit (EXIT_FAILURE);
        }

        // the card name matches the standard form, install it
        card = install_card (d -> devname, strtol (d -> devname + strlen ("/dev/dio144-"), NULL, 16));
        ncards++;

        /*
         *  Now verify the configuration and configure the card.
         *  The DIO-144 is a flexible configuration card, with the 
         *  channels set up as follows:
         *
         *      Channels 0 through 5 consist of three ports (A, B, and C)
         *      Ports A and B can be (independant of each other) all digital in or all digital out
         *      Port C can be split on a nybble basis to be inputs or outputs
         *      No other channels are supported.
        */

        memset (config, 0, sizeof (config));        // wipe the configuration

        for (nc = 0; nc < d -> nchannels; nc++) {
            parser_channel_t    *c;
            int                 np;

            c = &d -> channels [nc];
            for (np = 0; np < c -> nports; np++) {
                parser_port_t   *p;
                int             ndio;
                parser_aio_t    *aio;
                parser_dio_t    *dio;

                p = &c -> ports [np];
                i = atoi (c -> chan);

                // there are no analog channels
                if (p -> naios) {
                    aio = &p -> aios [0];           // just grab the first one for diagnostics
                    fprintf (stderr, "%s:  channel %d (tag %s) is listed as an analog port, but the DIO-144 does not have any analog ports\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                    exit (EXIT_FAILURE);
                }

                // check (and configure) all digital channels and ports
                if (i < 0 || i > 5) {
                    dio = &p -> dios [0];           // just grab the first one for diagnostics
                    fprintf (stderr, "%s:  channel %d (tag %s) is out of range {0..5} for the DIO-144\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                    exit (EXIT_FAILURE);
                }

                for (ndio = 0; ndio < p -> ndios; ndio++) {
                    int             portindex;

                    dio = &p -> dios [ndio];

                    // verify the bit number as being in range 0..7
                    if (dio -> bitnum < 0 || dio -> bitnum > 7) {
                        fprintf (stderr, "%s:  channel %d (tag %s) bit number (%d) is not in range {0..7}\n", progname, i, dio -> tag ? dio -> tag : "<none>", dio -> bitnum);
                        exit (EXIT_FAILURE);
                    }

                    portindex = 0;
                    switch (tolower (*p -> port)) { // first character of port better be an A, B, or C
                    case    'c':
                        portindex++;
                        if (dio -> bitnum > 3) {
                            portindex++;            // go to Chigh
                        }
                        // fall through
                    case    'b':
                        portindex++;
                        // fall through
                    case    'a':
                        if (config [i][portindex]) {        // if port is already configured...
                            if (dio -> direction) { // and this point has a configuration...
                                if (config [i][portindex] != dio -> direction) {
                                    fprintf (stderr, "%s:  channel %d (tag %s) port %s is already configured as %s and cannot be changed\n", progname, i, dio -> tag ? dio -> tag : "<none>", portnames [portindex], config [i][0] == 'i' ? "input" : "output");
                                    exit (EXIT_FAILURE);
                                }
                            } else {                // but point is not configured...
                                dio -> direction = config [i][portindex];
                            }
                        } else {                    // else if port is not already configured...
                            if (dio -> direction) { // and this point has a configuration...
                                config [i][portindex] = dio -> direction;
                            } else {
                                config [i][portindex] = dio -> direction = 'i'; // defaults to input
                            }
                        }
                        break;
                    default:
                        fprintf (stderr, "%s:  channel %d (tag %s) port (%s) is not one of A, B, or C\n", progname, i, dio -> tag ? dio -> tag : "<none>", *p -> port ? "<empty>" : p -> port);
                        exit (EXIT_FAILURE);
                    }
                }
            }
        }

        /*
         *  Now that we have the entire configuration for the card in our
         *  database, configure the card.
        */

        for (i = 0; i < 6; i++) {
            int     j;

            for (j = 0; j < 4; j++) {
                if (!config [i][j]) {
                    config [i][j] = 'i';
                }
            }
            dio144_configure_channel_port (card, i, 0, config [i][0] == 'i' ? DIO144_INPUT : DIO144_OUTPUT);
            if (config [i][0] == 'o') {
                dio144_write_channel_port (card, i, 0, opt01 ? 0xff : 0);
            }
            dio144_configure_channel_port (card, i, 1, config [i][1] == 'i' ? DIO144_INPUT : DIO144_OUTPUT);
            if (config [i][1] == 'o') {
                dio144_write_channel_port (card, i, 1, opt01 ? 0xff : 0);
            }
            dio144_configure_channel_port (card, i, 2, config [i][2] == 'i' ? DIO144_INPUT_LOW : DIO144_OUTPUT_LOW);
            dio144_configure_channel_port (card, i, 2, config [i][3] == 'i' ? DIO144_INPUT_HIGH : DIO144_OUTPUT_HIGH);
            if (config [i][2] == 'o' || config [i][3] == 'o') {
                if (config [i][2] == 'o' && config [i][3] == 'i') {
                    dio144_write_channel_port (card, i, 2, opt01 ? 0x0f : 0);
                }
                if (config [i][2] == 'i' && config [i][3] == 'o') {
                    dio144_write_channel_port (card, i, 2, opt01 ? 0xf0 : 0);
                }
                if (config [i][2] == 'o' && config [i][3] == 'o') {
                    dio144_write_channel_port (card, i, 2, opt01 ? 0xff : 0);
                }
            }
        }
    }
    return (ncards);
}

int
dio144_read_channel (dio144_t *dio, int c)
{
    int     base;

    c %= NCHANS;                // guarantee range

    base = c * PORTSPAN + dio -> port;

    return (in16 (base) + in8 (base + 2) * 65536);
}

int
dio144_read_channel_port (dio144_t *dio, int c, int p)
{
    c %= NCHANS;                // guarantee range
    p %= NPORTSPERCHAN;         // guarantee range

    return (in8 (c * 4 + p + dio -> port));
}

void
dio144_write_channel (dio144_t *dio, int c, int value)
{
    int     base;

    c %= NCHANS;                // guarantee range

    base = c * PORTSPAN + dio -> port;
    dio -> dout [c] = value;    // stash so we can do bit ops

    out16 (base, value & 0xffff);
    out8 (base + 2, (value & 0xff0000) >> 16);
}

void
dio144_write_channel_port (dio144_t *dio, int c, int p, int value)
{
    c %= NCHANS;                // guarantee range
    p %= NPORTSPERCHAN;         // guarantee range

    // overwrite appropriate byte in shadow register
    switch (p) {
    case    0:
        dio -> dout [c] = (dio -> dout [c] & 0xffff00) | (value & 0xff);
        break;
    case    1:
        dio -> dout [c] = (dio -> dout [c] & 0xff00ff) | ((value & 0xff) << 8);
        break;
    case    2:
        dio -> dout [c] = (dio -> dout [c] & 0x00ffff) | ((value & 0xff) << 16);
        break;
    }

    out8 (c * PORTSPAN + p + dio -> port, value & 0xff);
}

static int bits [24] = {
    0x000001, 0x000002, 0x000004, 0x000008, 0x000010, 0x000020, 0x000040, 0x000080,
    0x000100, 0x000200, 0x000400, 0x000800, 0x001000, 0x002000, 0x004000, 0x008000,
    0x010000, 0x020000, 0x040000, 0x080000, 0x100000, 0x200000, 0x400000, 0x800000
};

void
dio144_write_channel_port_bit (dio144_t *dio, int c, int p, int bitnum, int bitval)
{
    bitnum &= 7;            // guarantee range
    c %= NCHANS;
    p %= NPORTSPERCHAN;

    // adjust shadow first
    if (bitval) {
        dio -> dout [c] |= bits [bitnum + p * 8];
    } else {
        dio -> dout [c] &= ~bits [bitnum + p * 8];
    }

    switch (p) {
    case    0:
        out8 (c * PORTSPAN + p  + dio -> port,  dio -> dout [c] & 0x0000ff);
        break;
    case    1:
        out8 (c * PORTSPAN + p  + dio -> port, (dio -> dout [c] & 0x00ff00) >> 8);
        break;
    case    2:
        out8 (c * PORTSPAN + p  + dio -> port, (dio -> dout [c] & 0xff0000) >> 16);
        break;
    }
}

void
dio144_configure_channel_port (dio144_t *dio, int c, int p, int mode)
{
    int a, b, cl, ch;           // port statii; 0==out, 1==in

    c %= NCHANS;                // guarantee range
    p %= NPORTSPERCHAN;

    // get all the configurations, we'll reassemble them later
    a = !!(dio -> configs [c] & DIO144_CFG_PORT_A_IN);
    b = !!(dio -> configs [c] & DIO144_CFG_PORT_B_IN);
    ch = !!(dio -> configs [c] & DIO144_CFG_PORT_CH_IN);
    cl = !!(dio -> configs [c] & DIO144_CFG_PORT_CL_IN);

    switch (p) {
    case    0:      // port A
        if (mode == DIO144_OUTPUT) {
            a = 0;
        } else if (mode == DIO144_INPUT) {
            a = 1;
        }   // all others invalid, so no change
        break;

    case    1:      // port B
        if (mode == DIO144_OUTPUT) {
            b = 0;
        } else if (mode == DIO144_INPUT) {
            b = 1;
        }   // all others invalid, so no change
        break;

    case    2:      // port C
        if (mode & DIO144_O) {
            if (mode & DIO144_H) {
                ch = 0;
            }
            if (mode & DIO144_L) {
                cl = 0;
            }
        } else {
            if (mode & DIO144_H) {
                ch = 1;
            }
            if (mode & DIO144_L) {
                cl = 1;
            }
        }
        break;
    }

    dio -> configs [c] = DIO144_CFG_CONSTANT | (a * DIO144_CFG_PORT_A_IN) | (b * DIO144_CFG_PORT_B_IN) | (cl * DIO144_CFG_PORT_CL_IN) | (ch * DIO144_CFG_PORT_CH_IN);
    out8 (dio -> port + c * PORTSPAN + DIO144_CFG, dio -> configs [c]);
}
