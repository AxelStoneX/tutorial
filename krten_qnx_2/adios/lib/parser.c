
/*
 *  parser.c
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
 *  This module is the PARSER module used to parse the configuration
 *  file for ADIOS and its drivers.
 *
 *  2003 07 07  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fnmatch.h>
#include <ctype.h>
#include <errno.h>

#include "parser.h"

static  void                read_line (parser_handle_t *handle);
static  int                 get_next_token (parser_handle_t *h);
static  parser_device_t     *add_device (parser_t *out);
static  parser_channel_t    *add_channel (parser_device_t *device);
static  parser_port_t       *add_port (parser_channel_t *channel);
static  parser_aio_t        *add_aio (parser_port_t *port);
static  parser_dio_t        *add_dio (parser_port_t *port);
static  void                sort_database (parser_t *o);

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

/*
 *  Configuration file format
 *  =========================
 *
 *  The configuration file is a flat ASCII text file with multiple
 *  sections.  Each section defines the values for one particular
 *  card.  A complete configuration example is presented at the end
 *  of this section.
 *
 *  Configuration for the card ends when another card identifier
 *  is detected:
 *
 *  device /dev/device1
 *      ... configuration for /dev/device1
 *      ... more configuration for /dev/device1
 *      ... yet more configuration for /dev/device1
 *  device /dev/device2
 *      ... configuration for /dev/device2
 *      ... etc
 *
 *  Each card is divided into a number of channels.  Channel numbers are
 *  assigned in the order Analog In, Analog Out, Digital In, Digital Out.
 *  The configuration for each channel continues until another channel is
 *  selected:
 *
 *  channel 1
 *      ... configuration for channel 1
 *      ... more configuration for channel 1
 *  channel 2
 *      ... configuration for channel 2
 *      ... etc
 *
 *  Each channel consists of a number of ports, with the configuration
 *  for each port continuing until another port is selected:
 *
 *  port A
 *      ... configuration for port A
 *      ... more configuration for port A
 *  port B
 *      ... configuration for port B
 *      ... etc
 *
 *  Digital I/O channels are further divided into a number of bits,
 *  and the configuration information is given for the bits as above.
 *
 *  Finally, the "real" configuration information is presented for each
 *  port (or channel in the case of digital I/O).
 *
 *  Configuration values are selected from the following.
 *
 *  For digital channels:
 *
 *      bit <bitnumber> [din|dout] [positive|negative] [tag <tagname>]
 *
 *  For analog channels:
 *
 *      ain|aout [span <lowvalue>,<highvalue>] [gain <gainvalue>] [tag <tagname>]
 *
 *  A Complete Example
 *  ==================
 *
 *  A complete example of a tiny system with a PCL-711 card and a DIO-144
 *  card is given, followed by a commentary on the individual lines.  Note
 *  that the style is deliberately inconsistent in order to illustrate the
 *  flexibility.
 *
 *  device /dev/pcl711-0220
 *      channel 1 ain span 0,70 tag BOILER_TEMPERATURE
 *      channel 2
 *          ain
 *          span -3,+4.2285
 *          gain 2
 *          tag VOLTAGE_1
 *      channel 3 ain span +100,-100 tag PRESSURE_1
 *      # channels 4 and 5 are not used here
 *      channel 6 ain tag RAW_ANALOG_1
 *      channel
 *          7 ain
 *              span 0,1 tag spud
 *
 *      # Channel 8 is not used
 *
 *      # Channel 9 is the output channel
 *      channel 9 aout span 0,8191 tag OUTPUT_TEMPERATURE_1
 *
 *      # Channel 10 is the digital input channel with 8 bits
 *      channel 10 
 *          bit 0 din negative tag CHECK_1
 *          bit 1 din positive tag CHECK_2
 *          din bit 2 positive tag SWITCH_1
 *          din bit 3 positive tag SWITCH_2
 *          # bits 4 through 7 are not used
 *
 *      # Channel 11 is the digital output channel with 8 bits
 *      channel 11
 *          bit 0 dout positive tag CONVEYOR_MOTOR_1
 *          bit 1 negative tag CONVEYOR_START
 *          bit 2 negative tag ALARM_1_LED
 *          bit 3 dout negative tag ALARM_2_LED
 *          # bits 4 through 7 are not used
 *
 *  device /dev/dio144-0330
 *      channel 1 port a
 *          bit 0 din tag traffic_enable_switch
 *          din bit 1 tag emergency_stop_switch
 *          din bit 7 tag over_temperature
 *      port bit
 *          dout bit 0 tag te_led_1
 *          bit 1 negative te_led_bar_1
 *      port c
 *          bit 0 din tag x1
 *          bit 1 din tag x2
 *          bit 2 din tag x3
 *          bit 3 din tag x4
 *          bit 4 dout tag x5_out
 *          bit 5 tag x6_out
 *          # bit 6 is not used
 *          bit 7 tag x7_out
 *
 *  First, the PCL-711 card.
 *
 *  "device /dev/pcl711-0220" tells the pcl711 driver that it should install
 *  a handler for a PCL-711 card at I/O port 0x220.
 *
 *  "channel 1" indicates configuration data for channel 1, the first channel
 *  on the card.  Recall that the channel counting order is AI/AO/DI/DO, so
 *  channel 1 is the first analog input port.  This is confirmed by the
 *  required keyword "ain" indicating "Analog INput".
 *
 *  "span 0,70" indicates that the analog values from the card should be
 *  rescaled from their native 12-bit value (0,4095) to a value between 0
 *  and 70.  This rescaling is not done by the driver nor by ADIOS, but
 *  exists for higher level utilities to read and process the tag database.
 *
 *  "tag BOILER_TEMPERATURE" assigns the tag name "BOILER_TEMPERATURE" to this
 *  I/O point.
 *
 *  "channel 2" indicates that configuration for channel 1 is complete, and
 *  what follows is configuration data for channel 2 (the 2nd analog input
 *  channel on the PCL-711 card).
 *
 *  "span -3,+4.2285" indicates the span.
 *
 *  "gain 2" indicates that a card-specific gain value of "2" should be used.
 *
 *  "tag VOLTAGE_1" assigns the name of this I/O point.
 *
 *  "channel 3" indicates channel 2 configuration is done, and what follows
 *  is channel 3's configuration data.
 *
 *  "span +100,-100" indicates an inverted span (i.e., the raw analog value
 *  "0" corresponds to "+100" and the raw analog value "4095" corresponds
 *  to a value of "-100".  Keep in mind that this span conversion is not
 *  performed by ADIOS nor the driver.
 *
 *  "tag PRESSURE_1" assigns the name of this I/O point.
 *
 *  Note that we did not specify a gain value -- in this case, the default
 *  value of 0 is used.
 *
 *  Note also that channels 4 and 5 are not used, so we simply left them
 *  out of the configuration file.
 *
 *  "channel 6 ain tag RAW_ANALOG_1" illustrates assigning a tag to a raw
 *  analog value (i.e., no span conversion takes place, and the default gain
 *  value of 0 is used).
 *
 *  "channel 7" configuration simply shows that the configuration values can
 *  be split across multiple lines, and that the tag names do not have to
 *  be in all upper case.
 *
 *  Channel 8 is not used.
 *
 *  Channel 9 is the one (and only) analog output channel.  This is confirmed
 *  by the mandatory keyword "aout" (Analog OUT).
 *
 *  Starting with "channel 10", we see the definition for digital bits.
 *
 *  "bit 0 din negative tag CHECK_1" indicates that this is a bit,
 *  that it's bit 0, it's an input (which is redundant in this
 *  case because input is the default), and that its value should be
 *  inverted ("negative").  Finally, the tag "CHECK_1"
 *  is assigned to this I/O point.
 *
 *  "bit 1 din positive tag CHECK_2" and "din bit 2 positive tag SWITCH_1"
 *  illustrate two more bits that do not require inversion (hence "positive").
 *  (Notice the order of bit and din is unimportant).
 *
 *  The definition for bit 3 is similar.
 *
 *  Finally, channel 11 is the one (and only) digital output channel, as
 *  indicated by the "dout" (Digital OUTput) keyword.  Note that only the
 *  first bit needs to have the direction stated; the others default.
 *
 *  For the DIO-144 card, the process is similar.
 *
 *  The "device /dev/dio144-0330" informs the DIO-144 driver that it should
 *  install a handler for a DIO-144 device at I/O port 0x330.
 *
 *  Since the DIO-144 card has flexible configuration for its inputs and
 *  outputs, you need to tell it how a particular port is configured.  If you
 *  don't, an input is assumed.  Notice how the three defined bits, 0, 1, and
 *  7, all specify "din".  In reality, only the first bit need have
 *  specified this, but it's good practice to put in all the definitions.
 *
 *  For port B, the first bit is an "output" ("dout"), so the port is
 *  configured for output mode.  Notice how bit 1 in port B does not specify
 *  an input or output mode -- the mode defaults from the first bit (also,
 *  the card cannot be programmed to have some bits as inputs and some as
 *  outputs on ports A and B).  Port C is a different matter -- bits 0 through
 *  3 are programmed as inputs (defined by the first "din" on bit 0), and
 *  bits 4 through 7 are defined as outputs (defined by the first "dout" on
 *  bit 4).  Notice how bits 5 and 7 do not specify an input or output
 *  designation; they assume the default from bit 4.
*/

/*
 *  Configuration Parser API
 *  ========================
 *
 *  The basic idea of the configuration parser presented in this library
 *  is that a client will request information for one or more devices from
 *  the configuration file.  The information will be parsed by the parser
 *  into a dynamically allocated data structure.
 *
 *  Validation of the contents of this data structure are shared between
 *  the parser and the client.  The parser performs basic validation, such
 *  as ensuring that there are no duplicate tags, that all values are
 *  within generic specifications, etc.  It's then up to the client to
 *  verify that the type and ranges of arguments are suitable to that
 *  client.  For example, the parser has no idea of how many and what type
 *  of I/O are present on, let's say, a PCL-711 card.  The PCL-711 client
 *  will call the parser to convert the configuration into a set of data
 *  structures.  It will then be up to the client to apply its knowledge
 *  of the PCL-711's capabilities to the data structure returned by the
 *  parser.
 *
 *  The basic operation flow is:
 *
 *      fp = parser_open (fname);
 *      if (data = parser_parse (fp, "name of my device")) {
 *          // do your own processing here
 *          parser_free (data);
 *      }
 *      parser_close (fp);
 *
 *  The parser_open() function can also take a NULL value, which will cause
 *  it to search the default filename ("adios.cfg" in the current directory,
 *  then in "/etc").
 *
 *  parser_parse() reads all of the matching devices (the device name can
 *  use the "*" wildcard character) and creates a data structure.
 *
 *  Calling parser_free() after the parser_parse() releases the allocated
 *  data structures.
*/

#define ST_EOF                          -1
#define ST_RESET                        0
#define ST_DEVICE                       1

parser_handle_t *
parser_open (char *fname)
{
    parser_handle_t *handle;
    FILE            *fp;
    char            *f;

    if (fname) {
        fp = fopen (fname, "r");
        if (fp) {
            f = strdup (fname);
        }
    } else {
        fp = fopen ("/etc/adios.cfg", "r");
        if (fp) {
            f = strdup ("/etc/adios.cfg");
        } else {
            fp = fopen ("adios.cfg", "r");
            if (fp) {
                f = strdup ("adios.cfg");
            }
        }
    }

    handle = NULL;
    if (fp) {
        handle = malloc (sizeof (*handle));
        memset (handle, 0, sizeof (*handle));
        handle -> fp = fp;
        handle -> fname = f;
        handle -> state = ST_RESET;
        read_line (handle);
    }
        
    return (handle);
}

void
parser_close (parser_handle_t *handle)
{
    if (handle) {
        fclose (handle -> fp);
        memset (handle, 0, sizeof (*handle));
        free (handle);
    }
}

#define TOK_UNKNOWN     -2      // unknown token
#define TOK_EOF         -1      // reached the end of input; please make it stop!
#define TOK_DEVICE      1       // device <devicename>
#define TOK_CHANNEL     2       // channel <channelnumber>
#define TOK_PORT        3       // port <portname>
#define TOK_BIT         4       // bit <bitnumber>
#define TOK_AIN         5       // analog input
#define TOK_AOUT        6       // analog output
#define TOK_DIN         7       // digital input
#define TOK_DOUT        8       // digital output
#define TOK_POSITIVE    9       // positive
#define TOK_NEGATIVE    10      // negative
#define TOK_TAG         11      // tag <tagname>
#define TOK_SPAN        12      // span <lowvalue>,<highvalue>
#define TOK_GAIN        13      // gain <gainvalue>

/*
 *  parser_parse
 *
 *  This is the configuration file parser.  It builds up a structure
 *  of type "parser_t" which contains entries from the configuration
 *  file that match the "devname" parameter.  Since the "devnmae"
 *  parameter can include the wildcard "*", this may match multiple
 *  (if not all) devices within the configuration file.
 *
 *  Theory of Operation
 *  -------------------
 *  The parser gets tokens from the input stream.  Depending on the
 *  token, and the current state, an action is selected.  The current
 *  state is derived by looking at the pointers device, channel, port,
 *  aio, and dio.  Since these pointers point to data structures that
 *  have a hierarchical relationship, it is possible to derive the
 *  current state by observing the NULL or non-NULL value of the pointers.
 *  For example, if the "device" and "channel" pointers are non-NULL,
 *  and a TOK_DEVICE is seen on the input, then the parser will throw
 *  away the "channel" pointer (set it to NULL), and go to the next
 *  device.  If, on the other hand, in the same situation, a TOK_CHANNEL
 *  was seen, then the parser would move on to the next "channel" pointer
 *  and ensure that port, aio, and dio pointers were NULL.
 *
 *  The parser is a little unweildy at the moment, being around 250 lines.
 *  I'm thinking it can have a "change_state (tok, &dev, &chan, &port, &dio, &aio)
 *  kind of thing that's table driven.  There are also lots of common code
 *  sequences and checks that can be modularized.  Perhaps just break it
 *  up into a jump-table for the token and have individual handlers (breaks
 *  the fall-through in BIT/DIN/DOUT, though...)
*/

parser_t *
parser_parse (parser_handle_t *handle, char *devname)
{
    int                 tok;
    int                 skip_device;
    char                *cptr;
    parser_t            *out;
    parser_device_t     *device;
    parser_channel_t    *channel;
    parser_port_t       *port;
    parser_aio_t        *aio;
    parser_dio_t        *dio;

    out = malloc (sizeof (*out));
    if (out == NULL) {
        fprintf (stderr, "*** [library %s %d]:  parse_device was unable to allocate more memory (%d bytes), errno %d (%s)\n", __FILE__, __LINE__, sizeof (*out), errno, strerror (errno));
        return (NULL);
    }
    memset (out, 0, sizeof (*out));

    // reset our pointers (also used for state)
    device = NULL;
    channel = NULL;
    port = NULL;
    aio = NULL;
    dio = NULL;

    out -> status = PARSER_OK;

    skip_device = 0;                // set this when skipping a device that doesn't match "devname"

    // do the entire file
    while (handle -> state != ST_EOF) {

        tok = get_next_token (handle);

        switch (tok) {

        case    TOK_AIN:
        case    TOK_AOUT:
            if (skip_device) {
                break;
            }
            aio = NULL;                     // an "AIN" or "AOUT" token forces the start of the next analog I/O point
            if (!port && !channel) {
                fprintf (stderr, "*** %s line %d:  detected an %s but no PORT or CHANNEL\n", handle -> fname, handle -> lnum, tok == TOK_AIN ? "AIN" : "AOUT");
                out -> status = PARSER_MISSING_PORT_OR_CHANNEL;
                return (out);
            }
            if (!port) {
                port = add_port (channel);  // we allow portless analog I/O points
            }
            aio = add_aio (port);
            aio -> direction = tok == TOK_AIN ? 'i' : 'o';
            dio = NULL;
            break;

        case    TOK_BIT:
            get_next_token (handle);        // get bit number out of the input stream
            dio = NULL;                     // a "BIT" token *FORCES* the start of the next digital I/O point; DIN/DOUT just create one if it doesn't exist
            // fall through for common processing
        case    TOK_DIN:
        case    TOK_DOUT:
            if (skip_device) {
                break;
            }
            if (!port && !channel) {
                fprintf (stderr, "*** %s line %d:  detected a BIT but no PORT or CHANNEL\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_PORT_OR_CHANNEL;
                return (out);
            }
            if (!port) {
                port = add_port (channel);  // we allow portless digital I/O points
                dio = NULL;
            }
            if (!dio) {
                dio = add_dio (port);
            }
            aio = NULL;
            if (tok == TOK_BIT) {
                dio -> bitnum = atoi (handle -> token);
            } else {
                dio -> direction = tok == TOK_DIN ? 'i' : 'o';
            }
            break;

        case    TOK_CHANNEL:
            get_next_token (handle);        // get channel designation out of the input stream
            if (skip_device) {
                break;
            }
            if (!device) {
                fprintf (stderr, "*** %s line %d:  detected a CHANNEL but no DEVICE\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_DEVICE;
                return (out);
            }
            channel = add_channel (device);
            strncpy (channel -> chan, handle -> token, MAX_CHAN_NAME);      // strncpy is safe here because a new channel from add_channel is nul'd
            // zero out the others; they aren't valid any longer
            port = NULL;
            aio = NULL;
            dio = NULL;
            break;

        case    TOK_DEVICE:
            get_next_token (handle);        // get device name out of the input stream
            if (!fnmatch (devname, handle -> token, 0)) {
                device = add_device (out);
                device -> devname = strdup (handle -> token);
                skip_device = 0;
            } else {
                device = NULL;              // didn't match, therefore this is no longer valid
                skip_device = 1;
            }
            // zero out the others; they aren't valid any longer
            channel = NULL;
            port = NULL;
            aio = NULL;
            dio = NULL;
            break;

        case    TOK_GAIN:
            get_next_token (handle);
            if (skip_device) {
                break;
            }
            if (!port && !channel) {
                fprintf (stderr, "*** %s line %d:  detected a GAIN but no PORT or CHANNEL\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_PORT_OR_CHANNEL;
                return (out);
            }
            if (!aio) {
                fprintf (stderr, "*** %s line %d:  detected a GAIN but no AIN or AOUT\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_ANALOG;
                return (out);
            }
            aio -> gain = atoi (handle -> token);
            break;

        case    TOK_NEGATIVE:
        case    TOK_POSITIVE:
            if (skip_device) {
                break;
            }
            if (!port && !channel) {
                fprintf (stderr, "*** %s line %d:  detected a %s but no PORT or CHANNEL\n", handle -> fname, handle -> lnum, tok == TOK_NEGATIVE ? "NEGATIVE" : "POSITIVE");
                out -> status = PARSER_MISSING_PORT_OR_CHANNEL;
                return (out);
            }
            if (!dio) {
                fprintf (stderr, "*** %s line %d:  detected a %s but no BIT\n", handle -> fname, handle -> lnum, tok == TOK_NEGATIVE ? "NEGATIVE" : "POSITIVE");
                out -> status = PARSER_MISSING_DIGITAL;
                return (out);
            }
            dio -> polarity = tok == TOK_NEGATIVE ? '-' : '+';
            break;

        case    TOK_PORT:
            get_next_token (handle);
            if (skip_device) {
                break;
            }
            if (!channel) {
                fprintf (stderr, "*** %s line %d:  detected a PORT but no CHANNEL\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_PORT_OR_CHANNEL;
                return (out);
            }
            port = add_port (channel);
            strncpy (port -> port, handle -> token, MAX_PORT_NAME);
            break;

        case    TOK_SPAN:
            get_next_token (handle);
            if (skip_device) {
                break;
            }
            if (!port && !channel) {
                fprintf (stderr, "*** %s line %d:  detected a SPAN but no PORT or CHANNEL\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_PORT_OR_CHANNEL;
                return (out);
            }
            if (!aio) {
                fprintf (stderr, "*** %s line %d:  detected a SPAN but no AIN or AOUT\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_ANALOG;
                return (out);
            }
            if (strtok (handle -> token, ",")) {
                if (*handle -> token == '+') {
                    aio -> span_low = atof (&handle -> token [1]);
                } else {
                    aio -> span_low = atof (&handle -> token [0]);
                }
                if (!(cptr = strtok (NULL, ","))) {
                    fprintf (stderr, "*** %s line %d:  missing second value of SPAN (first value was %g)\n", handle -> fname, handle -> lnum, aio -> span_low);
                    out -> status = PARSER_MISSING_SPAN2;
                    return (out);
                }
                if (*cptr == '+') {
                    aio -> span_high = atof (cptr + 1);
                } else {
                    aio -> span_high = atof (cptr);
                }
            } else {
                fprintf (stderr, "*** %s line %d:  missing first value of SPAN\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_SPAN1;
                return (out);
            }
            // printf ("GOT SPAN %g to %g\n", aio -> span_low, aio -> span_high);
            break;

        case    TOK_TAG:
            get_next_token (handle);
            if (skip_device) {
                break;
            }
            if (!aio && !dio) {
                fprintf (stderr, "*** %s line %d:  detected a TAG but no AIN/AOUT/DIN/DOUT/BIT keyword\n", handle -> fname, handle -> lnum);
                out -> status = PARSER_MISSING_TAGGABLE;
                return (out);
            }
            if (parser_find_by_tag (out, handle -> token, NULL)) {
                fprintf (stderr, "*** %s line %d:  duplicate tag \"%s\"\n", handle -> fname, handle -> lnum, handle -> token);
                out -> status = PARSER_DUPLICATE_TAG;
                return (out);
            }
            if (aio) {
                aio -> tag = strdup (handle -> token);
            } else {
                dio -> tag = strdup (handle -> token);
            }
            break;

        case    TOK_UNKNOWN:
            fprintf (stderr, "*** %s line %d:  unknown keyword \"%s\"\n", handle -> fname, handle -> lnum, handle -> token);
            out -> status = PARSER_UNKNOWN_TOKEN;
            return (out);
        }
    }

    sort_database (out);

    return (out);
}

void
parser_free (parser_t *data)
{
}

void
parser_dump (parser_t *o)
{
    int                 nd;
    parser_device_t     *d;
    int                 nc;
    parser_channel_t    *c;
    int                 np;
    parser_port_t       *p;
    int                 naio;
    parser_aio_t        *aio;
    int                 ndio;
    parser_dio_t        *dio;

    printf ("Dumping parser tree at pointer %p\n", o);
    if (!o) {
        printf ("  Null\n");
        return;
    }

    printf ("%d devices:\n", o -> ndevices);
    for (nd = 0; nd < o -> ndevices; nd++) {
        d = &o -> devices [nd];
        printf ("DEVICE %d [%p], \"%s\" has %d channels\n", nd, d, d -> devname, d -> nchannels);
        for (nc = 0; nc < d -> nchannels; nc++) {
            c = &d -> channels [nc];
            printf ("  CHANNEL %d [%p], \"%s\" has %d ports\n", nc, c, c -> chan, c -> nports);
            for (np = 0; np < c -> nports; np++) {
                p = &c -> ports [np];
                printf ("    PORT %d [%p], \"%s\" has %d ANALOG and %d DIGITAL points\n", np, p, p -> port, p -> naios, p -> ndios);
                for (naio = 0; naio < p -> naios; naio++) {
                    aio = &p -> aios [naio];
                    printf ("      ANALOG  %d [%p] direction %c gain %d span (%g to %g) (TAG \"%s\")\n", naio, aio, aio -> direction, aio -> gain, aio -> span_low, aio -> span_high, aio -> tag ? aio -> tag : "<none>");
                }
                for (ndio = 0; ndio < p -> ndios; ndio++) {
                    dio = &p -> dios [ndio];
                    printf ("      DIGITAL %d [%p] bit %d polarity %d direction %c (TAG \"%s\")\n", ndio, dio, dio -> bitnum, dio -> polarity, dio -> direction, dio -> tag ? dio -> tag : "<none>");
                }
            }
        }
    }
    printf ("\n");
}

/*
 *  parser_find_by_tag
 *
 *  This is a utility routine that's used internally to detect duplicate
 *  tags, and can be called by the client to find an analog or digital
 *  I/O point by tag.
 *
 *  Note that we allow "anonymous" tags; i.e., tags that have no name.
 *  These will never be found by this function.
 *
 *  Operation is a straightforward linear search.  This is felt to be
 *  "good enough" as most tag searches will be done once when the
 *  client loads up its tag database.  For the case where we are adding
 *  tags to the database and checking for duplicates, this too is a
 *  "one of" operation -- if the tag database is large, then this can
 *  become a problem, and we will need to consider other ways of detecting
 *  duplicates.  One approach would be to only look for duplicates at
 *  the end of loading the database.  The line number where the tags are
 *  defined can be stored in the database, and then the report on duplicates
 *  can simply say something like "tag 'xyzzy' duplicated on lines A,
 *  B, and C".
*/

void *
parser_find_by_tag (parser_t *data, char *tag, int *type)
{
    int                 nd;
    parser_device_t     *d;
    int                 nc;
    parser_channel_t    *c;
    int                 np;
    parser_port_t       *p;
    int                 naio;
    parser_aio_t        *aio;
    int                 ndio;
    parser_dio_t        *dio;

    if (!data) {
        return (0);
    }

    for (nd = 0; nd < data -> ndevices; nd++) {
        d = &data -> devices [nd];
        for (nc = 0; nc < d -> nchannels; nc++) {
            c = &d -> channels [nc];
            for (np = 0; np < c -> nports; np++) {
                p = &c -> ports [np];
                for (naio = 0; naio < p -> naios; naio++) {
                    aio = &p -> aios [naio];
                    if (aio -> tag && !strcmp (aio -> tag, tag)) {
                        if (type) {
                            *type = 'a';
                        }
                        return (aio);
                    }
                }
                for (ndio = 0; ndio < p -> ndios; ndio++) {
                    dio = &p -> dios [ndio];
                    if (dio -> tag && !strcmp (dio -> tag, tag)) {
                        if (type) {
                            *type = 'd';
                        }
                        return (dio);
                    }
                }
            }
        }
    }
    return (0);
}

static const char *parser_error_strings [] =
{
    "No Error",                                                                                 // PARSER_OK
    "Keyword requires a preceeding \"port\" or \"channel\" keyword, but none was found",        // PARSER_MISSING_PORT_OR_CHANNEL
    "Keyword requires a preceeding \"device\", but none was found",                             // PARSER_MISSING_DEVICE
    "Keyword requires a preceeding analog keyword (\"ain\" or \"aout\"), but none was found",   // PARSER_MISSING_ANALOG
    "Keyword requires a preceeding digital keyword (\"bit\"), but none was found",              // PARSER_MISSING_DIGITAL
    "Missing first part (or all) of argument to \"span\" keyword",                              // PARSER_MISSING_SPAN1
    "Missing second part of argument to \"span\" keyword",                                      // PARSER_MISSING_SPAN2
    "A tag was found, but nothing to attach it to (an analog or digital I/O bit)",              // PARSER_MISSING_TAGGABLE
    "A duplicate tag was detected",                                                             // PARSER_DUPLICATE_TAG
    "A syntax error was encountered"                                                            // PARSER_UNKNOWN_TOKEN
};

const char *
parser_strerror (int e)
{
    if (e < 0 || e >= (sizeof (parser_error_strings) / sizeof (char *))) {
        return ("Bad error number");
    }
    return (parser_error_strings [e]);
}

/*
 *  ++++++++++++++++
 *  STATIC functions
 *  ++++++++++++++++
*/

/*
 *  Parse tree management:
 *
 *  add_device
 *  add_channel
 *  add_port
 *  add_aio
 *  add_dio
 *
 *  These routines add one more of the named object to the owner of the
 *  object.  For example, "add_channel" adds one more channel to the owner
 *  of the channels, which is the device.
*/

static parser_device_t *
add_device (parser_t *out)
{
    out -> devices = realloc (out -> devices, sizeof (*out -> devices) * (out -> ndevices + 1));
    if (out -> devices == NULL) {
        fprintf (stderr, "*** [library %s %d]:  add_device was unable to reallocate more memory (%d bytes), errno %d (%s)\n", __FILE__, __LINE__, sizeof (*out -> devices) * (out -> ndevices + 1), errno, strerror (errno));
        return (NULL);
    }
    memset (&out -> devices [out -> ndevices], 0, sizeof (parser_device_t));
    out -> devices [out -> ndevices].parser = out;              // back link
    return (&out -> devices [out -> ndevices++]);
}

static parser_channel_t *
add_channel (parser_device_t *device)
{
    device -> channels = realloc (device -> channels, sizeof (*device -> channels) * (device -> nchannels + 1));
    if (device -> channels == NULL) {
        fprintf (stderr, "*** [library %s %d]:  add_channel was unable to reallocate more memory (%d bytes), errno %d (%s)\n", __FILE__, __LINE__, sizeof (*device -> channels) * (device -> nchannels + 1), errno, strerror (errno));
        return (NULL);
    }
    memset (&device -> channels [device -> nchannels], 0, sizeof (parser_channel_t));
    device -> channels [device -> nchannels].device = device;   // back link
    return (&device -> channels [device -> nchannels++]);
}

static parser_port_t *
add_port (parser_channel_t *channel)
{
    channel -> ports = realloc (channel -> ports, sizeof (*channel -> ports) * (channel -> nports + 1));
    if (channel -> ports == NULL) {
        fprintf (stderr, "*** [library %s %d]:  add_port was unable to reallocate more memory (%d bytes), errno %d (%s)\n", __FILE__, __LINE__, sizeof (*channel -> ports) * (channel -> nports + 1), errno, strerror (errno));
        return (NULL);
    }
    memset (&channel -> ports [channel -> nports], 0, sizeof (parser_port_t));
    channel -> ports [channel -> nports].channel = channel;     // back link
    return (&channel -> ports [channel -> nports++]);
}

static parser_aio_t *
add_aio (parser_port_t *port)
{
    port -> aios = realloc (port -> aios, sizeof (*port -> aios) * (port -> naios + 1));
    if (port -> aios == NULL) {
        fprintf (stderr, "*** [library %s %d]:  add_aio was unable to reallocate more memory (%d bytes), errno %d (%s)\n", __FILE__, __LINE__, sizeof (*port -> aios) * (port -> naios + 1), errno, strerror (errno));
        return (NULL);
    }
    memset (&port -> aios [port -> naios], 0, sizeof (parser_aio_t));
    port -> aios [port -> naios].port = port;                   // back link
    return (&port -> aios [port -> naios++]);
}

static parser_dio_t *
add_dio (parser_port_t *port)
{
    port -> dios = realloc (port -> dios, sizeof (*port -> dios) * (port -> ndios + 1));
    if (port -> dios == NULL) {
        fprintf (stderr, "*** [library %s %d]:  add_dio was unable to reallocate more memory (%d bytes), errno %d (%s)\n", __FILE__, __LINE__, sizeof (*port -> dios) * (port -> ndios + 1), errno, strerror (errno));
        return (NULL);
    }
    memset (&port -> dios [port -> ndios], 0, sizeof (parser_dio_t));
    port -> dios [port -> ndios].port = port;                   // back link
    return (&port -> dios [port -> ndios++]);
}

/*
 *  Sorting of the parse tree (sort_database() function)
 *
 *  We should sort the database for easy processing by the client.
 *  We sort primarily by device, secondarily by channel, and thirdly
 *  by port.  This all comes out in the wash due to the structuring of the "for" loops.
 *
 *  There are several qsort() helper functions, for sorting their
 *  respective data types:
 *      sort_database_by_device()
 *      sort_database_by_channel()
 *      sort_database_by_port()
 *      sort_database_by_dio()
*/

static int
sort_database_by_device (const void *pa, const void *pb)
{
    const parser_device_t   *a = pa;
    const parser_device_t *b = pb;

    return (strcmp (a -> devname, b -> devname)); 
}

static int
sort_database_by_channel (const void *pa, const void *pb)
{
    const parser_channel_t *a = pa;
    const parser_channel_t *b = pb;

    return (strcmp (a -> chan, b -> chan)); 
}

static int
sort_database_by_port (const void *pa, const void *pb)
{
    const parser_port_t *a = pa;
    const parser_port_t *b = pb;

    return (strcmp (a -> port, b -> port)); 
}

static int
sort_database_by_dio (const void *pa, const void *pb)
{
    const parser_dio_t *a = pa;
    const parser_dio_t *b = pb;

    return (a -> bitnum - b -> bitnum); 
}

static void
sort_database (parser_t *o)
{
    int                 nd;
    parser_device_t     *d;
    int                 nc;
    parser_channel_t    *c;
    int                 np;
    parser_port_t       *p;
    int                 naio;
    parser_aio_t        *aio;
    int                 ndio;
    parser_dio_t        *dio;

    // sort by device name first
    qsort (o -> devices, o -> ndevices, sizeof (parser_device_t), sort_database_by_device);

    // for each device, sort by channel
    for (nd = 0; nd < o -> ndevices; nd++) {
        d = &o -> devices [nd];
        qsort (d -> channels, d -> nchannels, sizeof (parser_channel_t), sort_database_by_channel);

        // for each channel, sort by port
        for (nc = 0; nc < d -> nchannels; nc++) {
            c = &d -> channels [nc];
            qsort (c -> ports, c -> nports, sizeof (parser_port_t), sort_database_by_port);

            // note that we do not sort analog channels, because they are already sorted by port.

            // for each digital bit, sort by bit number
            for (np = 0; np < c -> nports; np++) {
                p = &c -> ports [np];
                qsort (p -> dios, p -> ndios, sizeof (parser_dio_t), sort_database_by_dio);
                
            }
        }
    }

    /*
     *  now we need to fixup the backlinks, because the qsort()
     *  algorithm copies data elements around, meaning that the
     *  backlinks don't point to what they used to.
    */

    for (nd = 0; nd < o -> ndevices; nd++) {
        d = &o -> devices [nd];
        for (nc = 0; nc < d -> nchannels; nc++) {
            c = &d -> channels [nc];
            c -> device = d;
            for (np = 0; np < c -> nports; np++) {
                p = &c -> ports [np];
                p -> channel = c;
                for (naio = 0; naio < p -> naios; naio++) {
                    aio = &p -> aios [naio];
                    aio -> port = p;
                }
                for (ndio = 0; ndio < p -> ndios; ndio++) {
                    dio = &p -> dios [ndio];
                    dio -> port = p;
                }
            }
        }
    }
}

#define SKIP_WHITESPACE             0
#define SKIP_TO_WHITESPACE_OR_EOL   1
#define DONE                        2

typedef struct token_s
{
    const char  *string;
    int         token;
}   token_t;

token_t     tokens [] =
{
    {"ain",         TOK_AIN},
    {"aout",        TOK_AOUT},
    {"bit",         TOK_BIT},
    {"channel",     TOK_CHANNEL},
    {"device",      TOK_DEVICE},
    {"din",         TOK_DIN},
    {"dout",        TOK_DOUT},
    {"gain",        TOK_GAIN},
    {"negative",    TOK_NEGATIVE},
    {"port",        TOK_PORT},
    {"positive",    TOK_POSITIVE},
    {"span",        TOK_SPAN},
    {"tag",         TOK_TAG}
};

static int
get_next_token (parser_handle_t *h)
{
    int     state;
    int     i;

    h -> token = NULL;                  // don't have one yet
    state = SKIP_WHITESPACE;
    while (state != DONE) {
        switch (state) {
        case    SKIP_WHITESPACE:
            if (isspace (*h -> bufptr)) {
                h -> bufptr++;
                break;
            }

            if (*h -> bufptr == '#') {  // a comment
                read_line (h);
                if (h -> state == ST_EOF) {
                    state = DONE;
                }
                break;
            }

            if (!*h -> bufptr) {
                read_line (h);
                if (h -> state == ST_EOF) {
                    state = DONE;
                }
                break;
            }

            h -> token = h -> bufptr;   // snap this "start" character
            state = SKIP_TO_WHITESPACE_OR_EOL;
            h -> bufptr++;
            break;

        case    SKIP_TO_WHITESPACE_OR_EOL:
            if (!*h -> bufptr || isspace (*h -> bufptr)) {
                *h -> bufptr = 0;           // single NUL terminate
                h -> bufptr++;              // skip past it to either a character or the second nul.
                state = DONE;
                break;
            }
            h -> bufptr++;
            break;
        }
    }

    // now analyze the token (if any)
    if (h -> token) {
        for (i = 0; i < sizeof (tokens) / sizeof (token_t); i++) {
            if (!stricmp (tokens [i].string, h -> token)) {
                return (tokens [i].token);
            }
        }
        return (TOK_UNKNOWN);
    }
    return (TOK_EOF);
}

static void
read_line (parser_handle_t *handle)
{
    if (!fgets (handle -> buffer, BUFSIZ, handle -> fp)) {
        handle -> state = ST_EOF;
    } else {
        handle -> buffer [strlen (handle -> buffer) - 1] = 0;   // ensure a double NUL and kill the \n
        handle -> lnum++;
    }
    handle -> bufptr = handle -> buffer;
}

