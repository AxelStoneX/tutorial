
/*
 *  iso813.h
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
 *  the ISO-813 driver.
 *
 *  2003 07 04  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

// include the common ADIOS driver specification

#include "../api/driver.h"
#include "parser.h"

/*
 *  manifest constants
*/

#define NAI                         32      // analog inputs
#define AI_RESOLUTION               12      // analog input is 12 bits
#define AIGAINMIN                   0       // minimum gain value
#define AIGAINMAX                   4       // maximum gain value

/*
 *  The ISO813_TIMEOUT value is the number of times that we try to read from
 *  the ISO-813's analog input port, looking for the Data Ready bit to go
 *  active (zero).  On a Pentium II running at 350 MHz, a typical number of
 *  times to read the port is 12 to 15, so the number below should work for a
 *  large range of machines.  Also, if the analog value cannot be read, then
 *  an error indication is returned.
*/

#define ISO813_TIMEOUT              200     // number of times we try to read the analog port before giving up.
#define ISO813_DELAY                1000    // 1000 ns = 1 us

/*
 *  iso813_t
 *
 *  This contains the information about each card.  The global variable
 *  "iso813" is built initially when we process the command line options,
 *  and then may have stuff added to it at runtime.
 *
 *  It is implemented as an "extended attributes structure" in terms of
 *  the resource manager side of things.
*/

typedef struct iso813_s
{
    iofunc_attr_t   attr;           // attributes structure
    int             port;           // base port, e.g. 0x220
    char            *name;          // registered name, e.g. /dev/iso813-0220
    int             gains [NAI];    // gain values for all analog inputs (in native ISO-813 format, i.e., 0..4 not 1x, 2x, 4x, 8x, 16x)
}   iso813_t;


/*
 *  ISO-813 register definitions
*/

#define ISO813_AD_LOW               0x04    // (read) base + 0x04 is the low byte
#define ISO813_AD_HIGH              0x05    // (read) base + 0x05 is the high byte and status
#define ISO813_AD_GAIN              0x09    // (write) gain control output
#define ISO813_AD_MUX               0x0a    // (write) multiplexer control
#define ISO813_AD_TRIGGER           0x0c    // (write) trigger a conversion

#define ISO813_AD_HIGH_DRDY         0x10    // bit 0x10 is 0 when data is ready

/*
 *  prototypes
*/

extern  iso813_t    *install_card (char *name, int port);
extern  int         install_cards_from_database (parser_t *data);
extern  int         iso813_read_analog_channel (iso813_t *iso, int channel);
extern  void        iso813_set_gain (iso813_t *iso, int channel, int gaincode);
extern  int         iso813_get_gain (iso813_t *iso, int channel);
extern  void        calibrate_cards (void);

