
/*
 *  iso813.c
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
 *  This module contains the ISO-813 functionality.
 *
 *  2003 07 04  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

//#define TESTING   // define this if testing the calibration mode without a card

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <hw/inout.h>

#include "iso813.h"

extern  char    *progname;                      // main.c
extern  char    *version;                       // version.c

extern  int     optd;                           // main.c, debug/diag mode
extern  int     optv;                           // main.c

iso813_t        *iso813;                        // global information structure about all cards (see iso813.h)
int             niso813;                        // how many are in the structure above

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

iso813_t *
install_card (char *name, int port)
{
    iso813_t        *p;

    iso813 = realloc (iso813, sizeof (iso813_t) * (niso813 + 1));
    if (iso813 == NULL) {
        fprintf (stderr, "%s:  out of memory trying to add port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    p = &iso813 [niso813++];

    memset (p, 0, sizeof (*p));

    p -> port = port;
    if (!name) {
        name = strdup ("/dev/iso813-xxxx");     // create one big enough for the generated name
        if (!name) {
            fprintf (stderr, "%s:  out of memory trying to generate name for port 0x%x (errno %d (%s))\n", progname, port, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
        sprintf (name, "/dev/iso813-%04x", port);   // now put in the real name
        p -> name = name;
    } else {
        p -> name = strdup (name);  // create copy of theirs
    }

    iofunc_attr_init (& p -> attr, S_IFREG | 0666, 0, 0);
    p -> attr.inode = niso813;                  // inode is index + 1 (inode 0 means "not in use)

    /*
     *  Verify that we have a "live" board, or at least a piece
     *  of hardware that looks like an ISO-813 board.  This is
     *  done by trying to read an analog value.
    */

    if (iso813_read_analog_channel (p, 0) < 0) {        // ooops; not a good board!
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
        int i;
        int j;
    
        j = 100;
        for (i = 0; i < NAI; i++) {
            iso813_set_gain (p, i, 0);
        }
        while (1)   {
            if (j++ > 20) {
                j = 0;
                printf ("\n");
                for (i = 0; i < NAI; i++) {
                    printf ("C%02d ", i);
                }
                printf ("\n");
                for (i = 0; i < NAI; i++) {
                    printf ("--- ");
                }
                printf ("\n");
            }
            for (i = 0; i < NAI; i++) {
                printf ("%03X ", iso813_read_analog_channel (p, i));
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
 *  the ISO-813 card.  While the parser_parse() function does some
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
    iso813_t            *card;

    // ensure we have something to work on
    if (!data) {
        return (0);     // not an error -- we just didn't have anything in the database
    }
    ncards = 0;

    for (nd = 0; nd < data -> ndevices; nd++) {
        parser_device_t *d;
        int             nc;

        d = &data -> devices [nd];
        if (strncmp (d -> devname, "/dev/iso813-", strlen ("/dev/iso813-"))) {
            fprintf (stderr, "%s:  device name \"%s\" is not of the form \"/dev/iso813-xxxx\" and therefore cannot be configured automatically from configuration file\n", progname, d -> devname);
            exit (EXIT_FAILURE);
        }

        // the card name matches the standard form, install it
        card = install_card (d -> devname, strtol (d -> devname + strlen ("/dev/iso813-"), NULL, 16));
        ncards++;

        /*
         *  Now verify the configuration and configure the card.
         *  The ISO-813 is a fixed configuration card only, with the 
         *  channels set up as follows:
         *
         *      Channels 0 through 31 are always AIN
         *      No other channels are supported.
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

                    // we have an analog channel, verify 0-31 are AIN
                    i = atoi (c -> chan);
                    if (i < 0 || i > 31) {
                        fprintf (stderr, "%s:  channel %d (tag %s) is out of range; should be 0..31\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                        exit (EXIT_FAILURE);
                    }

                    if (1 <= i && i <= 8) {         // channels 1 through 32 better be inputs
                        if (!aio -> direction) {    // wants to default, let it
                            aio -> direction = 'i';
                        }
                        if (aio -> direction != 'i') {
                            fprintf (stderr, "%s:  channel %d (tag %s) is not an analog input channel and the ISO-813 requires that channels 1..32 be analog inputs\n", progname, i, aio -> tag ? aio -> tag : "<none>");
                            exit (EXIT_FAILURE);
                        }

                        // verified.  It's an input and we're happy.  Fetch the gain.
                        if (aio -> gain < AIGAINMIN || aio -> gain > AIGAINMAX) {
                            fprintf (stderr, "%s:  channel %d (tag %s) has a gain value (%d) that's out of range {%d..%d}\n", progname, i, aio -> tag ? aio -> tag : "<none>", aio -> gain, AIGAINMIN, AIGAINMAX);
                            exit (EXIT_FAILURE);
                        }

                        // gain verified, configure it.
                        iso813_set_gain (card, i - 1, aio -> gain);
                    }
                }

                // check all digital channels
                for (ndio = 0; ndio < p -> ndios; ndio++) {
                    parser_dio_t *dio;

                    dio = &p -> dios [ndio];

                    // We don't support any digital I/O on this card...
                    i = atoi (c -> chan);
                    fprintf (stderr, "%s:  channel %d (tag %s) is a digital I/O point; the ISO-813 card only supports analog input!\n", progname, i, dio -> tag ? dio -> tag : "<none>");
                    exit (EXIT_FAILURE);
                }
            }
        }
    }
    return (ncards);
}

/*
 *  iso813_read_analog_channel (iso813, channel)
 *
 *  Reads the analog value from the analog to digital convertor.
 *  If the DRDY bit is set to one, we wait -- this is because the conversion
 *  takes a maximum of 25 microseconds (in practice, on a Pentium 2 @ 350 MHz,
 *  there were between 13 and 15 iterations of the do/while loop).
 *
 *  The gain used for the channel is stored in the iso813_t structure and
 *  is fetched from there.
 *
 *  The value 0x000 -> 0xFFF is returned.
*/

int
iso813_read_analog_channel (iso813_t *iso, int channel)
{
    int     data;
    int     base;
    static  struct timespec when;
    static  struct timespec ten_microseconds;
    static  int initialized = 0;
    int     timeout;

    /*
     *  Set up the nanospin "timespec" structure to be 10 microseconds.
     *  It's done once only; the value is 10 thousand nanoseconds.
    */

    if (!initialized) {
        nanospin_calibrate (1);
        nsec2timespec (&when, ISO813_DELAY);
        nsec2timespec (&when, 10000);       // 10000 us == 10 us
        initialized = 1;
    }

    channel &= 31;                          // guarantee range
    base = iso -> port;

    out8 (base + ISO813_AD_MUX, channel);

    nanospin (&ten_microseconds);           // manual page 13 sez to wait at least 10 us

    out8 (base + ISO813_AD_GAIN, iso -> gains [channel]);

    nanospin (&ten_microseconds);           // manual page 15 sez to wait at least 10 us here too

    out8 (base + ISO813_AD_TRIGGER, 0 /* any data */);

    timeout = ISO813_TIMEOUT;
    do {
        data = in8 (base + ISO813_AD_HIGH);
        nanospin (&when);
    } while ((data & ISO813_AD_HIGH_DRDY) && (timeout-- >= 0));

    if (timeout < 0) {
        return (-1);
    }
    data = ((data & 0x0f) << 8) + in8 (base + ISO813_AD_LOW);
    return (data);
}

void
iso813_set_gain (iso813_t *iso, int channel, int gaincode)
{
    if (gaincode < 0 || gaincode > 4) {
        return;
    }
    channel &= 31;
    iso -> gains [channel] = gaincode;
}

int
iso813_get_gain (iso813_t *iso, int channel)
{
    channel &= 31;
    return (iso -> gains [channel]);
}

/*
 *  calibration_cards
 *
 *  This is a text menu-driven calibration function that
 *  is modelled after the text starting on page 27 of the
 *  ISO-813 manual.
*/

static iso813_t * menu_select_card (void);
static void calibrate_card (iso813_t *c);
static void await_calibrated_value (iso813_t *c, int v, int vr, int chan);

void
calibrate_cards (void)
{
    printf ("\n\n*** CALIBRATION MODE ***\n------------------------\n");

#ifdef TESTING
    printf ("@@@@@@@ @@@@@@@  @@@@@  @@@@@@@   @@@   @     @  @@@@@\n");
    printf ("   @    @       @     @    @       @    @@    @ @     @\n");
    printf ("   @    @       @          @       @    @ @   @ @\n");
    printf ("   @    @@@@@    @@@@@     @       @    @  @  @ @  @@@@\n");
    printf ("   @    @             @    @       @    @   @ @ @     @\n");
    printf ("   @    @       @     @    @       @    @    @@ @     @\n");
    printf ("   @    @@@@@@@  @@@@@     @      @@@   @     @  @@@@@\n");
#endif // TESTING
    for (;;) {
        calibrate_card (menu_select_card ());
    }
}

static iso813_t *
menu_select_card (void)
{
    int     i;
    char    buf [BUFSIZ];
    
    printf ("Please select one of the following options, and hit return:\n");
    for (i = 0; i < niso813; i++) {
        printf ("  %2d) Calibrate card named \"%s\"\n", i, iso813 [i].name);
    }
    printf ("   x) exit\n");

    while (fgets (buf, BUFSIZ, stdin) && !feof (stdin)) {
        if (tolower (*buf) == 'x') {
            break;
        }
        if (isdigit (*buf)) {
            i = atoi (buf);
            if (i >= 0 && i < niso813) {
                return (&iso813 [i]);
            }
        }
        printf ("\nSorry, that is not a valid selection (must be 0..%d or \"x\")\n", niso813 - 1);
    }
    printf ("\nExiting program\n");
    exit (EXIT_SUCCESS);
}

static void
calibrate_card (iso813_t *c)
{
    char    buf [BUFSIZ];

    printf ("We will be following the procedures as outlined in the ISO-813\n");
    printf ("manual, starting on page 27.\n");
    printf ("\n");
    printf ("YOU MUST ENSURE THAT JP1 IS IN THE UP POSITION (10V input range setting)\n");
    printf ("AND THAT JP2 IS IN THE DOWN POSITION (bipolar setting)\n");
    printf ("Hit return when you are ready, ^D to abort\n");
    printf ("\n");
    fgets (buf, BUFSIZ, stdin);
    if (feof (stdin)) {
        printf ("\nExiting program\n");
        exit (EXIT_SUCCESS);
    }

    printf ("\n");
    printf ("Now, apply 0V to AI channel 0, and 4.9988V to AI channel 1.\n");
    printf ("Hit return when you are ready, ^D to abort, s to skip these\n");
    printf ("tests and continue on to UNIPOLAR adjustment mode\n");
    printf ("\n");
    fgets (buf, BUFSIZ, stdin);
    if (feof (stdin)) {
        printf ("\nExiting program\n");
        exit (EXIT_SUCCESS);
    }

    if (tolower (*buf) != 's') {
        await_calibrated_value (c, 2047, 1, 0);
        await_calibrated_value (c, 4094, 2, 1);
        await_calibrated_value (c, 2047, 4, 0);
    }

    printf ("\n");
    printf ("If you are using bipolar input range in your application, you can skip\n");
    printf ("this step.  Simply type 'x' followed by return.  A plain return will\n");
    printf ("continue onto the offset adjustment\n");

    fgets (buf, BUFSIZ, stdin);
    if (feof (stdin)) {
        printf ("\nExiting program\n");
        exit (EXIT_SUCCESS);
    }
    if (tolower (*buf) == 'x') {
        return;
    }

    printf ("\n");
    printf ("Since we placed the card into BIPOLAR mode for the tests above, and you have\n");
    printf ("indicated that you will be using the card in UNIPOLAR mode, you must now\n");
    printf ("reconfigure the card for UNIPOLAR mode.  The manual suggests shutting down\n");
    printf ("the PC, taking the ISO-813 card out, putting JP2 into the UP position,\n");
    printf ("reinstalling the ISO-813 card and restarting the test procedure.  You\n");
    printf ("may skip the first three calibration steps by hitting 's' at the menu.\n");

    printf ("Hit return when you are ready, ^D to abort\n");
    printf ("\n");
    fgets (buf, BUFSIZ, stdin);
    if (feof (stdin)) {
        printf ("\nExiting program\n");
        exit (EXIT_SUCCESS);
    }
    await_calibrated_value (c, 0, 3, 0);

    printf ("\n\nCARD CALIBRATION COMPLETED!\n\n");
}

static void
await_calibrated_value (iso813_t *c, int v, int vr, int chan)
{
    int     nconsecutive;
    int     val;

    printf ("\n");
    printf ("Now adjust VR%d until the reading below is %d or %d...\n", vr, v, v + 1);
    printf ("(The program will automatically continue to the next step after 20 consecutive\n");
    printf ("readings that match the range specification have been sampled).\n");
    printf ("Hit ^C to terminate\n");

    nconsecutive = 0;
    while (nconsecutive < 20) {
        delay (100);
        val = iso813_read_analog_channel (c, chan);
        printf ("%04d\r", val); fflush (stdout);
#ifdef  TESTING
        nconsecutive++;
#else   // TESTING
        if (val == v || val == (v + 1)) {
            nconsecutive++;
        } else {
            nconsecutive = 0;
        }
#endif
    }
    printf ("\n");
}

