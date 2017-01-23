
/*
 *  parser.h
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
 *  the PARSER module used for ADIOS and its drivers.
 *
 *  2003 07 07  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#ifndef __PARSER_H_
#define __PARSER_H_

/*
 *  parser_handle_t
 *
 *  This is the data structure that users of the parser pass around
 *  as a handle.  It is created by parser_open(), used by a number
 *  of the internal routines as well as the parser_parse() routine,
 *  and closed by parser_close().
*/

typedef struct parser_handle_s
{
    // information about the input file
    FILE        *fp;
    char        *fname;
    int         lnum;

    // the parsing operation
    char        buffer [BUFSIZ];        // input buffer
    char        *bufptr;
    char        *token;
    int         state;

}   parser_handle_t;

/*
 *  Hierarchy of data structures returned after parsing a configuration
 *  file:
 *
 *  parser_t points to "ndevices" worth of "parser_device_t"
 *
 *  parser_device_t points to "nchannels" worth of "parser_channel_t"
 *
 *  parser_channel_t points to "nports" worth of "parser_port_t"
 *
 *  parser_port_t points to "ndios" worth of parser_dio_t and "naios"
 *  worth of parser_aio_t
 *
 *  Finally, parser_dio_t and parser_aio_t contain information about
 *  the endpoints.
 *
 *  You'll notice that in some cases the name of the entity is a
 *  pointer to a character string (e.g., char *devname) whereas in other
 *  cases it's an array of 4 characters (e.g., char port [MAX_PORT_NAME + 1]).
 *  This is done because the longer names (i.e., those that are pointers
 *  to strings) need to be allocated from memory, whereas things like ports
 *  and channels are always going to be one or two characters (three at
 *  the most) long, so the overhead of using a pointer is the same as the
 *  data required to actually store the information, let alone the cost
 *  of the allocated memory.
*/

#define MAX_PORT_NAME       3
#define MAX_CHAN_NAME       3

struct parser_port_s;
struct parser_channel_s;
struct parser_device_s;
struct parser_s;

typedef struct parser_dio_s
{
    struct parser_port_s    *port;                      // back link

    int                     bitnum;
    int                     polarity;
    int                     direction;
    char                    *tag;
}   parser_dio_t;

typedef struct parser_aio_s
{
    struct parser_port_s    *port;                      // back link

    int                     direction;
    int                     gain;
    double                  span_low, span_high;
    char                    *tag;
}   parser_aio_t;

typedef struct parser_port_s
{
    struct parser_channel_s *channel;                   // back link

    // information about this port
    char                    port [MAX_PORT_NAME + 1];   // see above

    // information about things attached to this port
    int                     ndios;
    parser_dio_t            *dios;

    int                     naios;
    parser_aio_t            *aios;
}   parser_port_t;

typedef struct parser_channel_s
{
    struct parser_device_s  *device;                    // back link

    // information about this channel
    char                    chan [MAX_CHAN_NAME + 1];   // see above

    // information about things attached to this channel
    int                     nports;
    parser_port_t           *ports;
}   parser_channel_t;

typedef struct parser_device_s
{
    struct parser_s         *parser;                    // back link

    // information about this device
    char                    *devname;

    // information about things attached to this device
    int                     nchannels;
    parser_channel_t        *channels;
}   parser_device_t;

typedef struct parser_s
{
    int                     status;                     // return status from parser, see PARSER_*
    int                     ndevices;
    parser_device_t         *devices;
}   parser_t;

/*
 *  Return status from parser, stored in "parser_t -> status" by
 *  parser_parse()
*/

#define PARSER_OK                           0           // everything is just peachy
#define PARSER_MISSING_PORT_OR_CHANNEL      1           // the keyword detected requires a preceeding "port" or "channel" keyword, but none was found
#define PARSER_MISSING_DEVICE               2           // the keyword detected requires a preceeding "device", but none was found
#define PARSER_MISSING_ANALOG               3           // the keyword detected requires a preceeding analog keyword ("ain" or "aout"), but none was found
#define PARSER_MISSING_DIGITAL              4           // the keyword detected requires a preceeding digital keyword ("bit"), but none was found
#define PARSER_MISSING_SPAN1                5           // missing first part (or all) of argument to "span" keyword
#define PARSER_MISSING_SPAN2                6           // missing second part of argument to "span" keyword
#define PARSER_MISSING_TAGGABLE             7           // a tag was found, but nothing to attach it to (an analog or digital I/O bit)
#define PARSER_DUPLICATE_TAG                8           // a duplicate tag was detected
#define PARSER_UNKNOWN_TOKEN                9           // a syntax error was encountered

/*
 *  Prototypes for public functions
*/

parser_handle_t             *parser_open (char *fname);
void                        parser_close (parser_handle_t *handle);
parser_t                    *parser_parse (parser_handle_t *handle, char *device);
void                        parser_free (parser_t *data);
void                        parser_dump (parser_t *data);
void                        *parser_find_by_tag (parser_t *data, char *tag, int *type);
const char                  *parser_strerror (int e);

// Debug
#define CP  printf ("[%s %d]", __FILE__, __LINE__); fflush (stdout);

#endif  // __PARSER_H_

