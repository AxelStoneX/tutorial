
/*
 *  iofuncs.c
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
 *  This module contains the I/O and Connect functions for the
 *  DIO-144 resource manager.
 *
 *  2003 07 04  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>                         // for io_read_dir()
#include <ctype.h>

#include "iofuncs.h"

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

extern  dio144_t    *dio144;                // dio144.c, global information structure about all cards (see dio144.h)
extern  int         ndio144;                // dio144.c, how many are in the structure above

int
io_devctl (resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    int                 sts;
    void                *data;
    int                 nbytes;
    int                 dcmd;
    int                 len;
    int                 i, j, k;
    int                 channel, port;
    dcmd_get_config_t   *dio144_get_config;
    dcmd_get_adis_t     *dio144_get_adis;
    dcmd_set_cpbdo_t    *dio144_set_cpbdo;
    uint8_t             *p8;

/*
 *  WARNING:  check in main.c to see if you need to adjust the "nparts_max" and "msg_max_size"
*/

    // see if it's a standard POSIX-supported devctl()
    if ((sts = iofunc_devctl_default (ctp, msg, ocb)) != _RESMGR_DEFAULT) {
        return (sts);
    }

    // assign a pointer to the data area of the message
    data = _DEVCTL_DATA (*msg);

    // get other data that can't be wiped out before we wipe the data structure
    dcmd = msg -> i.dcmd;
    len = msg -> i.nbytes;
    memset (&msg -> o, 0, sizeof (msg -> o));

    // preset the number of bytes that we'll return to zero
    nbytes = 0;

    // check for all commands
    switch (dcmd) {
    case    DCMD_GET_CONFIG:
        dio144_get_config = (dcmd_get_config_t *) data;
        memset (dio144_get_config, 0, sizeof (*dio144_get_config));
        dio144_get_config -> o.version = atof (version) * 1000;

        /*
         *  Note that this *looks* wrong, in that we may have a different number
         *  of configured inputs versus outputs.  However, by "lying" and saying
         *  that we have all our ports configured as both, we can maintain the
         *  layout the same without having ports "move" depending on the config.
        */

        dio144_get_config -> o.ndi = NCHANS * NPORTSPERCHAN * 8;
        dio144_get_config -> o.ndo = NCHANS * NPORTSPERCHAN * 8;
        dio144_get_config -> o.nao = 0;
        dio144_get_config -> o.nai = 0;
        dio144_get_config -> o.nbpc = NPORTSPERCHAN;
        dio144_get_config -> o.maxresai = 0;
        dio144_get_config -> o.maxresao = 0;
        nbytes = sizeof (*dio144_get_config);
        break;

    case    DCMD_GET_ADIS:
        dio144_get_adis = (dcmd_get_adis_t *) data;
        p8 = (uint8_t *) dio144_get_adis -> o.buf;
        for (i = 0; i < NCHANS; i++) {
            for (j = 0; j < NPORTSPERCHAN; j++) {
                *p8++ = dio144_read_channel_port (ocb -> attr, i, j);
                nbytes++;
            }
        }
        break;

    case    DCMD_SET_CPAO:
        return (EINVAL);        // no analog on this card!

    case    DCMD_SET_CPBDO:     // set digital channel/port.  Channel E {0..5}, port E {a, b, c}
        dio144_set_cpbdo = (dcmd_set_cpbdo_t *) data;

        for (k = 0; k < dio144_set_cpbdo -> i.ndos; k++) {
            channel = atoi (dio144_set_cpbdo -> i.dval [k].channel);
            if (channel < 0 || channel > 5) {
                return (EINVAL);
            }
            port = tolower (*dio144_set_cpbdo -> i.dval [k].port);
            if (dio144_set_cpbdo -> i.dval [k].port [1]) {
                return (EINVAL);    // only expect one character
            }
            if (port != 'a' && port != 'b' && port != 'c') {
                return (EINVAL);    // and it had better be A, B, or C
            }
            dio144_write_channel_port_bit (ocb -> attr, channel, port - 'a', dio144_set_cpbdo -> i.dval [k].bitnum, dio144_set_cpbdo -> i.dval [k].value);
        }
        break;

    default:
        return (ENOSYS);
    }

    // return data (if any) to the client
    msg -> o.nbytes = nbytes;
    SETIOV (ctp -> iov, &msg -> o, sizeof (msg -> o) + nbytes);
    return (_RESMGR_NPARTS (1));
}

