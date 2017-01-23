
/*
 *  dio144.h
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
 *  the DIO-144 driver.
 *
 *  2003 06 17  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

// include the common ADIOS driver specification

#include "../api/driver.h"
#include "parser.h"

/*
 *  manifest constants
*/

#define NCHANS                      6       // number of channels, each with...
#define NPORTSPERCHAN               3       // this many ports per channel
#define PORTSPAN                    4       // multiplier for base address

/*
 *  Mode settings
 *
 *  The finest granularity for port direction settings is 4 bits (a
 *  nybble, not a "nipple" as documented in the manual on page 16 :-))
 *
 *  The "OUTPUT" or "INPUT" designation applies for all 8 bits, whereas
 *  the "_HIGH" or "_LOW" suffixes mean that the mode setting applies
 *  to only the high (0xf0) or low (0x0f) nybble.
*/

#define DIO144_O                    0x10
#define DIO144_I                    0x20
#define DIO144_H                    0x01
#define DIO144_L                    0x02

#define DIO144_OUTPUT_HIGH          (DIO144_O | DIO144_H)
#define DIO144_OUTPUT_LOW           (DIO144_O | DIO144_L)
#define DIO144_OUTPUT               (DIO144_O | DIO144_H | DIO144_L)
#define DIO144_INPUT_HIGH           (DIO144_I | DIO144_H)
#define DIO144_INPUT_LOW            (DIO144_I | DIO144_L)
#define DIO144_INPUT                (DIO144_I | DIO144_H | DIO144_L)

#define DIO144_CFG                  3       // register offset 3
#define DIO144_CFG_CONSTANT         0x80    // constant value that must be applied to CFG register
#define DIO144_CFG_PORT_A_IN        0x10    // enable PORT A as input
#define DIO144_CFG_PORT_B_IN        0x02    // enable PORT B as input
#define DIO144_CFG_PORT_CL_IN       0x01    // enable PORT C low nybble as input
#define DIO144_CFG_PORT_CH_IN       0x08    // enable PORT C high nybble as input

/*
 *  dio144_t
 *
 *  This contains the information about each card.  The global variable
 *  "dio144" is built initially when we process the command line options,
 *  and then may have stuff added to it at runtime.
 *
 *  It is implemented as an "extended attributes structure" in terms of
 *  the resource manager side of things.
*/

typedef struct dio144_s
{
    iofunc_attr_t   attr;           // attributes structure
    int             port;           // base port, e.g. 0x2D0
    char            *name;          // registered name, e.g. /dev/dio144-02d0
    int             dout [NCHANS];  // value of digital output (shadow, to allow single-bit manipulations)
    uint8_t         configs [NCHANS];   // same as CFG register, see DIO144_CFG_* manifests above
}   dio144_t;


/*
 *  prototypes
*/

extern  dio144_t    *install_card (char *name, int port);
extern  int         install_cards_from_database (parser_t *data);
extern  int         dio144_read_channel (dio144_t *dio, int c);
extern  int         dio144_read_channel_port (dio144_t *dio, int c, int p);
extern  void        dio144_write_channel (dio144_t *dio, int c, int value);
extern  void        dio144_write_channel_port (dio144_t *dio, int c, int p, int value);
extern  void        dio144_write_channel_port_bit (dio144_t *dio, int c, int p, int bitnum, int bitval);
extern  void        dio144_configure_channel_port (dio144_t *dio, int c, int p, int mode);
