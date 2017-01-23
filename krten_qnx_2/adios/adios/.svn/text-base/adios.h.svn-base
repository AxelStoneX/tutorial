
/*
 *  adios.h
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
 *  This module contains the manifest constants and declarations for
 *  the Analog / Digital I/O Server (ADIOS).
 *
 *  2003 06 20  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <inttypes.h>

#include "driver.h"

#include "parser.h"

#include "daq.h"

/*
 *  adios_t
 *
 *  This contains the information about each device.  The global variable
 *  "adios" is built initially when we process the command line options,
 *  and then may have stuff added to it at runtime.
*/

typedef struct adios_s
{
    char            *name;          // registered name, e.g. /dev/pcl711-02d0
    int             nai, nao;       // number of analog inputs / outputs
    int             ndi, ndo;       // number of digital inputs / outputs
    int             nbpc;           // number of bytes per channel (e.g., 2 for PCL-711's 16 bit, 2 for DIO-144 -- DIGITAL ONLY)
    int             maxresai;       // maximum bit resolution of analog input (# bits)
    int             maxresao;       // maximum bit resolution of analog output (# bits)
    int             version;        // version of driver
    int             fd;             // an opened handle
}   adios_t;

extern  int     add_device (char *name, int fd);
extern  void    install_drivers_from_database (parser_t *p);

