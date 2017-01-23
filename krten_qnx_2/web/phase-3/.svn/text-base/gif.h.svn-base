
/*
 *  gif.h
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
 *  the GIF encoder.
 *
 *  2003 04 16  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

typedef unsigned char uchar;

#define MAXTEST   200
#define MAXENTRY  4093          /* a prime number is best for hashing */
#define MAXSTRING 64000

typedef struct gif_context_s
{
    int     strlocn [MAXENTRY];
    int     entrynum [MAXENTRY];
    uchar   teststring [MAXTEST];
    int     lentest;
    int     lastentry;
    int     numentries;
    int     numrealentries;
    int     nextentry;
    int     clearcode;
    int     endcode;
    int     hashcode;
    uchar   block [266];
    int     startbits;
    int     codebits;
    int     bytecount;
    int     bitcount;
    uchar   extra [65536];
}   gif_context_t;

extern  int     encode_image (unsigned char *raster, int x, int y, unsigned char *output);

