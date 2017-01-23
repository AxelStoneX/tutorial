
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
 *  SIM-000 resource manager.
 *
 *  2003 06 20  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>                         // for io_read_dir()

#include "iofuncs.h"

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

extern  sim000_t    *sim000;                // sim000.c, global information structure about all cards (see sim000.h)
extern  int         nsim000;                // sim000.c, how many are in the structure above

int
io_devctl (resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    int                 sts;
    void                *data;
    int                 nbytes;
    int                 dcmd;
    int                 len;
    int                 i, k;
    int                 channel;
    dcmd_get_config_t   *sim000_get_config;
    dcmd_get_adis_t     *sim000_get_adis;
    dcmd_set_cpao_t     *sim000_set_cpao;
    dcmd_set_cpbdo_t    *sim000_set_cpbdo;
    uint16_t            *p16;
    int                 v;

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
        sim000_get_config = (dcmd_get_config_t *) data;
        memset (sim000_get_config, 0, sizeof (*sim000_get_config));
        sim000_get_config -> o.version = atof (version) * 1000;
        sim000_get_config -> o.ndi = NDI;
        sim000_get_config -> o.ndo = NDO;
        sim000_get_config -> o.nao = NAO;
        sim000_get_config -> o.nai = NAI;
        sim000_get_config -> o.nbpc = NDI / 8;
        sim000_get_config -> o.maxresai = AI_RESOLUTION;
        sim000_get_config -> o.maxresao = AO_RESOLUTION;
        nbytes = sizeof (*sim000_get_config);
        break;

    case    DCMD_GET_ADIS:
        sim000_get_adis = (dcmd_get_adis_t *) data;
        p16 = (uint16_t *) sim000_get_adis -> o.buf;
        for (i = 0; i < NAI; i++) {
            v = sim000_read_analog (ocb -> attr, i);
            if (v < 0) {
                fprintf (stderr, "%s:  sim000_read_analog FAILED (port %s)!\n", progname, ocb -> attr -> name);
            }

            *p16++ = v;
        }
        *p16 = sim000_read_digital (ocb -> attr);
        nbytes = sizeof (uint16_t) * (NAI + 1);
        break;

    case    DCMD_SET_CPAO:
        sim000_set_cpao = (dcmd_set_cpao_t *) data;

        // it's a bit silly for "i.naos" to be anything other than "1", because we only have the one channel...
        for (k = 0; k < sim000_set_cpao -> i.naos; k++) {
            channel = atoi (sim000_set_cpao -> i.aval [k].channel);
            if (channel != 8) {
                return (EINVAL);        // only channel 8 is AOUT
            }
            sim000_write_analog (ocb -> attr, sim000_set_cpao -> i.aval [k].value);
        }
        break;

    case    DCMD_SET_CPBDO:
        sim000_set_cpbdo = (dcmd_set_cpbdo_t *) data;

        
        for (k = 0; k < sim000_set_cpbdo -> i.ndos; k++) {
            channel = atoi (sim000_set_cpbdo -> i.dval [k].channel);
            if (channel != 10) {
                return (EINVAL);
            }
            sim000_write_digital_bit (ocb -> attr, sim000_set_cpbdo -> i.dval [k].bitnum, sim000_set_cpbdo -> i.dval [k].value);
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

