
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
 *  PCL-711 resource manager.
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

extern  pcl711_t    *pcl711;                // pcl711.c, global information structure about all cards (see pcl711.h)
extern  int         npcl711;                // pcl711.c, how many are in the structure above

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
    dcmd_get_config_t   *pcl711_get_config;
    dcmd_get_adis_t     *pcl711_get_adis;
    dcmd_set_cpao_t     *pcl711_set_cpao;
    dcmd_set_cpbdo_t    *pcl711_set_cpbdo;
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
        pcl711_get_config = (dcmd_get_config_t *) data;
        memset (pcl711_get_config, 0, sizeof (*pcl711_get_config));
        pcl711_get_config -> o.version = atof (version) * 1000;
        pcl711_get_config -> o.ndi = NDI;
        pcl711_get_config -> o.ndo = NDO;
        pcl711_get_config -> o.nao = NAO;
        pcl711_get_config -> o.nai = NAI;
        pcl711_get_config -> o.nbpc = NDI / 8;
        pcl711_get_config -> o.maxresai = AI_RESOLUTION;
        pcl711_get_config -> o.maxresao = AO_RESOLUTION;
        nbytes = sizeof (*pcl711_get_config);
        break;

    case    DCMD_GET_ADIS:
        pcl711_get_adis = (dcmd_get_adis_t *) data;
        p16 = (uint16_t *) pcl711_get_adis -> o.buf;
        for (i = 0; i < NAI; i++) {
            v = pcl711_read_analog (ocb -> attr, i);
            if (v < 0) {
                fprintf (stderr, "%s:  pcl711_read_analog FAILED (port %s)!\n", progname, ocb -> attr -> name);
            }

            *p16++ = v;
        }
        *p16 = pcl711_read_digital (ocb -> attr);
        nbytes = sizeof (uint16_t) * (NAI + 1);
        break;

    case    DCMD_SET_CPAO:
        pcl711_set_cpao = (dcmd_set_cpao_t *) data;

        // it's a bit silly for "i.naos" to be anything other than "1", because we only have the one channel...
        for (k = 0; k < pcl711_set_cpao -> i.naos; k++) {
            channel = atoi (pcl711_set_cpao -> i.aval [k].channel);
            if (channel != 8) {
                return (EINVAL);        // only channel 8 is AOUT
            }
            pcl711_write_analog (ocb -> attr, pcl711_set_cpao -> i.aval [k].value);
        }
        break;

    case    DCMD_SET_CPBDO:
        pcl711_set_cpbdo = (dcmd_set_cpbdo_t *) data;

        
        for (k = 0; k < pcl711_set_cpbdo -> i.ndos; k++) {
            channel = atoi (pcl711_set_cpbdo -> i.dval [k].channel);
            if (channel != 10) {
                return (EINVAL);
            }
            pcl711_write_digital_bit (ocb -> attr, pcl711_set_cpbdo -> i.dval [k].bitnum, pcl711_set_cpbdo -> i.dval [k].value);
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

