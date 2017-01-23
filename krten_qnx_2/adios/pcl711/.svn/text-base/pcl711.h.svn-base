
/*
 *  pcl711.h
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
 *  the PCL-711 driver.
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

#define NAI                         8       // analog inputs
#define NAO                         1       // analog outputs
#define NDI                         16      // digital inputs
#define NDO                         16      // digital outputs
#define AI_RESOLUTION               12      // analog input is 12 bits
#define AO_RESOLUTION               12      // analog output is 12 bits
#define AIGAINMIN                   0       // minimum gain value
#define AIGAINMAX                   4       // maximum gain value

/*
 *  The PCL711_TIMEOUT value is the number of times that we try to read from
 *  the PCL-711's analog input port, looking for the Data Ready bit to go
 *  active (zero).  On a Pentium II running at 350 MHz, a typical number of
 *  times to read the port is 12 to 15, so the number below should work for a
 *  large range of machines.  Also, if the analog value cannot be read, then
 *  an error indication is returned.
*/

#define PCL711_TIMEOUT              200     // number of times we try to read the analog port before giving up.
#define PCL711_DELAY                1000    // 1 microsecond (1000 ns)

/*
 *  pcl711_t
 *
 *  This contains the information about each card.  The global variable
 *  "pcl711" is built initially when we process the command line options,
 *  and then may have stuff added to it at runtime.
 *
 *  It is implemented as an "extended attributes structure" in terms of
 *  the resource manager side of things.
*/

typedef struct pcl711_s
{
    iofunc_attr_t   attr;           // attributes structure
    int             port;           // base port, e.g. 0x2D0
    char            *name;          // registered name, e.g. /dev/pcl711-02d0
    int             dout;           // value of digital output (shadow, to allow single-bit manipulations)
    int             gains [NAI];    // gain values for all analog inputs (in native PCL-711 format, i.e., 0..4 not 1x, 2x, 4x, 8x, 16x)
}   pcl711_t;


/*
 *  PCL 711 register definitions
*/

#define PCL711_COUNTER_0            0       // I/O
#define PCL711_COUNTER_1            1       // I/O
#define PCL711_COUNTER_2            2       // I/O
#define PCL711_COUNTER_CONTROL      3       // O
#define PCL711_ANALOG_LOW           4       // I == A/D, O == D/A
#define PCL711_ANALOG_HIGH          5       // I == A/D, O == D/A
#define PCL711_DI_LOW               6       // I
#define PCL711_DI_HIGH              7       // I
#define PCL711_CLEAR_INTERRUPT      8       // O
#define PCL711_GAIN_CONTROL         9       // O
#define PCL711_MUX_SCAN_CONTROL     10      // O
#define PCL711_MODE_AND_INTERRUPT   11      // O
#define PCL711_SOFTWARE_AD_TRIGGER  12      // O
#define PCL711_DO_LOW               13      // O
#define PCL711_DO_HIGH              14      // O

// PCL711_ANALOG_HIGH register
#define PCL711_ANALOG_HIGH_DRDY     0x10    // DRDY bit set high if conversion in progress else low.

/*
 *  prototypes
*/

extern  pcl711_t    *install_card (char *name, int port);
extern  int         install_cards_from_database (parser_t *data);
extern  int         pcl711_read_analog (pcl711_t *pcl, int channel);
extern  void        pcl711_write_analog (pcl711_t *pcl, int value);
extern  int         pcl711_read_digital (pcl711_t *pcl);
extern  void        pcl711_write_digital (pcl711_t *pcl, int value);
extern  void        pcl711_write_digital_bit (pcl711_t *pcl, int bitnum, int bitval);
extern  void        pcl711_set_gain (pcl711_t *pcl, int channel, int gaincode);
extern  int         pcl711_get_gain (pcl711_t *pcl, int channel);

