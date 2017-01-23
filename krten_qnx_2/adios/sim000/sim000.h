
/*
 *  sim000.h
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
 *  the SIM-000 driver.
 *
 *  2003 10 09  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

// include the common ADIOS driver specification

#include "../api/driver.h"
#include "parser.h"

/*
 *  manifest constants; looks just like a PCL-711
*/

#define NAI                         8       // analog inputs
#define NAO                         1       // analog outputs
#define NDI                         16      // digital inputs
#define NDO                         16      // digital outputs
#define AI_RESOLUTION               12      // analog input is 12 bits
#define AO_RESOLUTION               12      // analog output is 12 bits
#define AIGAINMIN                   0       // minimum gain value
#define AIGAINMAX                   4       // maximum gain value

/*
 *  sim000_t
 *
 *  This contains the information about each card.  The global variable
 *  "sim000" is built initially when we process the command line options,
 *  and then may have stuff added to it at runtime.
 *
 *  It is implemented as an "extended attributes structure" in terms of
 *  the resource manager side of things.
*/

typedef struct sim000_s
{
    iofunc_attr_t   attr;           // attributes structure
    int             port;           // base port, e.g. 0x2D0
    char            *name;          // registered name, e.g. /dev/sim000-02d0
    int             dout;           // value of digital output (shadow, to allow single-bit manipulations)
    int             gains [NAI];    // not really used...
}   sim000_t;


/*
 *  prototypes
*/

extern  sim000_t    *install_card (char *name, int port);
extern  int         install_cards_from_database (parser_t *data);
extern  int         sim000_read_analog (sim000_t *sim, int channel);
extern  void        sim000_write_analog (sim000_t *sim, int value);
extern  int         sim000_read_digital (sim000_t *sim);
extern  void        sim000_write_digital (sim000_t *sim, int value);
extern  void        sim000_write_digital_bit (sim000_t *sim, int bitnum, int bitval);
extern  void        sim000_set_gain (sim000_t *sim, int channel, int gaincode);
extern  int         sim000_get_gain (sim000_t *sim, int channel);

