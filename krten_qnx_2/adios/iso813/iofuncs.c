
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
 *  ISO-813 resource manager.
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

#include "iofuncs.h"

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

extern  iso813_t    *iso813;                // iso813.c, global information structure about all cards (see iso813.h)
extern  int         niso813;                // iso813.c, how many are in the structure above

int
io_devctl (resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    int                 sts;
    void                *data;
    int                 nbytes;
    int                 dcmd;
    int                 len;
    int                 i;
    dcmd_get_config_t   *iso813_get_config;
    dcmd_get_adis_t     *iso813_get_adis;
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
        iso813_get_config = (dcmd_get_config_t *) data;
        memset (iso813_get_config, 0, sizeof (*iso813_get_config));
        iso813_get_config -> o.version = atof (version) * 1000;
        iso813_get_config -> o.ndi = 0;
        iso813_get_config -> o.ndo = 0;
        iso813_get_config -> o.nao = 0;
        iso813_get_config -> o.nai = NAI;
        iso813_get_config -> o.nbpc = 0;
        iso813_get_config -> o.maxresai = AI_RESOLUTION;
        iso813_get_config -> o.maxresao = 0;
        nbytes = sizeof (*iso813_get_config);
        break;

    case    DCMD_GET_ADIS:
        iso813_get_adis = (dcmd_get_adis_t *) data;
        p16 = (uint16_t *) iso813_get_adis -> o.buf;
        for (i = 0; i < NAI; i++) {
            v =  iso813_read_analog_channel (ocb -> attr, i);
            if (v < 0) {
                fprintf (stderr, "%s:  iso813_read_analog_channel FAILED (port %s)!\n", progname, ocb -> attr -> name);
            }
            *p16++ = v;
        }
        nbytes = sizeof (uint16_t) * NAI;
        
        break;

    case    DCMD_SET_CPAO:
    case    DCMD_SET_CPBDO:         // no digital on this card!
        return (EINVAL);            // no analog output on this card!

    default:
        return (ENOSYS);
    }

    // return data (if any) to the client
    msg -> o.nbytes = nbytes;
    SETIOV (ctp -> iov, &msg -> o, sizeof (msg -> o) + nbytes);
    return (_RESMGR_NPARTS (1));
}

