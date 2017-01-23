
/*
 *  adios.c
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
 *  This module represents the Analog / Digital I/O Server (ADIOS) module.
 *
 *  2003 06 20  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "adios.h"

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optv;                       // main.c

adios_t         *adios;                     // global information structure about all devices (see adios.h)
int             nadios;                     // how many are in the structure above

int
add_device (char *name, int fd)
{
    adios_t             *p;
    dcmd_get_config_t   c;
    int                 sts;
    int                 i;

    /*
     *  See if the device already exists...
    */

    for (i = 0; i < nadios; i++) {
        if (!strcmp (adios [i].name, name)) {
            fprintf (stderr, "%s:  WARNING: device \"%s\" is already in the database, ignored\n", progname, name);
            return (0);
        }
    }
    
    adios = realloc (adios, sizeof (adios_t) * (nadios + 1));
    if (adios == NULL) {
        fprintf (stderr, "%s:  out of memory trying to add device \"%s\" (errno %d (%s))\n", progname, name, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    p = &adios [nadios++];

    memset (p, 0, sizeof (*p));

    p -> name = strdup (name);

    if (fd != -1) {
        p -> fd = fd;
    } else {
        p -> fd = open (name, O_RDWR);
        if (p -> fd == -1) {
            fprintf (stderr, "%s:  error opening \"%s\", errno %d (%s)\n", progname, name, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
    }

    printf ("%s:  ", name);
    fflush (stdout);

    sts = devctl (p -> fd, DCMD_GET_CONFIG, &c, sizeof (c), NULL);
    if (sts != EOK) {
        fprintf (stderr, "%s:  failed devctl DCMD_GET_CONFIG, errno %d (%s)\n", progname, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    p -> nai = c.o.nai;
    p -> nao = c.o.nao;
    p -> ndi = c.o.ndi;
    p -> ndo = c.o.ndo;
    p -> nbpc = c.o.nbpc;
    p -> maxresai = c.o.maxresai;
    p -> maxresao = c.o.maxresao;
    p -> version = c.o.version;

    if (optv) {
        printf ("version %d.%03d ai %d (%d-bit) ao %d (%d-bit) di %d do %d nbpc %d\n", c.o.version / 1000, c.o.version % 1000, c.o.nai, c.o.maxresai, c.o.nao, c.o.maxresao, c.o.ndi, c.o.ndo, c.o.nbpc);
    }
    return (1);
}

void
install_drivers_from_database (parser_t *p)
{
    int     i;
    int     fd;

    for (i = 0; i < p -> ndevices; i++) {
        if ((fd = open (p -> devices [i].devname, O_RDWR)) == -1) {
            continue;
        }
        add_device (p -> devices [i].devname, fd);
    }
}

