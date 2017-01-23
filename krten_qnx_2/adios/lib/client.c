
/*
 *  client.c
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
 *  This module represents the library functions for the client.
 *
 *  2003 07 11  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "client.h"

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

static  int     adios_set_tag (parser_t *data, char *tag, double value, int mode);

/*
 *  adios_set_tag_span (database, tag, value)
 *  adios_set_tag_raw (database, tag, value)
 *  adios_set_chan_port_bit (device, chan, port, bit, value)
 *  adios_set_chan_port_analog (device, chan, port, value)
 *
 *  These functions are the client's interface to writing values
 *  to the devices.
 *
 *  The first one is the "highest level" function, in that it converts
 *  the value from engineering units according to the spanlow/spanhigh
 *  values, and then calls adios_set_tag_raw().
 *
 *  The adios_set_tag_raw() function accepts a raw value, and can be
 *  used for analog or digital values.
 *
 *  Both of these will then call down to one of adios_set_chan_port_bit()
 *  or adios_set_chan_port_analog() depending on whether the I/O point
 *  is a digital or analog point.
*/

int
adios_set_tag_span (parser_t *data, char *tag, double value)
{
    return (adios_set_tag (data, tag, value, 'c'));
}

int
adios_set_tag_raw (parser_t *data, char *tag, double value)
{
    return (adios_set_tag (data, tag, value, 'r'));
}

int
adios_set_chan_port_bit (char *device, char *chan, char *port, int bit, int value)
{
    int                 fd;
    int                 sts;
    dcmd_set_cpbdo_t    cmd;

    if ((fd = open (device, O_WRONLY)) == -1) {
        return (errno);
    }

    memset (&cmd, 0, sizeof (cmd));
    cmd.i.ndos = 1;
    strncpy (cmd.i.dval [0].channel, chan, 3);
    strncpy (cmd.i.dval [0].port, port, 3);
    cmd.i.dval [0].bitnum = bit;
    cmd.i.dval [0].value = value;

    sts = devctl (fd, DCMD_SET_CPBDO, &cmd, sizeof (cmd), NULL);
    close (fd);
    return (sts);
}

int
adios_set_chan_port_analog (char *device, char *chan, char *port, int value)
{
    int                 fd;
    int                 sts;
    dcmd_set_cpao_t     cmd;

    if ((fd = open (device, O_WRONLY)) == -1) {
        return (errno);
    }

    memset (&cmd, 0, sizeof (cmd));
    cmd.i.naos = 1;
    strncpy (cmd.i.aval [0].channel, chan, 3);
    strncpy (cmd.i.aval [0].port, port, 3);
    cmd.i.aval [0].value = value;

    sts = devctl (fd, DCMD_SET_CPAO, &cmd, sizeof (cmd), NULL);
    close (fd);
    return (sts);
}

static int
adios_set_tag (parser_t *data, char *tag, double value, int mode)
{
    void                *tmp;
    int                 type;

    if (!(tmp = parser_find_by_tag (data, tag, &type))) {
        return (ENXIO);
    }

    if (type == 'd') {
        parser_dio_t    *d;

        d = (parser_dio_t *) tmp;
        return (adios_set_chan_port_bit (d -> port -> channel -> device -> devname, d -> port -> channel -> chan, d -> port -> port, d -> bitnum, value != 0.0));
    } else if (type == 'a') {
        parser_aio_t    *a;

        a = (parser_aio_t *) tmp;
        if (mode == 'c' && (a -> span_high != a -> span_low)) {
            value = (value - a -> span_low) / (a -> span_high - a -> span_low) * 4095;  // ASSUMPTION: all analog outputs are 12 bits!!!
        }
        // if the mode is not "covert", or span low and high values are the same (usu. both 0), assume this means "raw" and don't touch "value"
        return (adios_set_chan_port_analog (a -> port -> channel -> device -> devname, a -> port -> channel -> chan, a -> port -> port, value));
    } else {
        return (EINVAL);
    }
}

