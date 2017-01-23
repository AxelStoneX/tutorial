
/*
 *  client.h
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
 *  the client data acquisition library for ADIOS.
 *
 *  2003 07 11  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#ifndef __CLIENT_H_
#define __CLIENT_H_

#include "driver.h"
#include "parser.h"

int                 adios_set_tag_span (parser_t *data, char *tag, double value);
int                 adios_set_tag_raw (parser_t *data, char *tag, double value);
int                 adios_set_chan_port_bit (char *device, char *chan, char *port, int bit, int value);
int                 adios_set_chan_port_analog (char *device, char *chan, char *port, int value);

#endif  // __CLIENT_H_

