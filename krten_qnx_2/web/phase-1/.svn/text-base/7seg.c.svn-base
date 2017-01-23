
/*
 *  7seg.c
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
 *  This module contains a 7-segment LED simulator that draws images
 *  into a memory buffer.
 *
 *  2003 03 25  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "7seg.h"

/*
 *  7 Segment Functions
 *
 *  The 7 segment digits are drawn by first finding the desired ASCII
 *  character (which must be a hexadecimal digit) in the seg7[] array.
 *  This array converts the character to the A, B, C, D, E, F, and G
 *  segments.  As per industry standard convention, the segments are
 *  labelled as follows:
 *
 *          a
 *         ---
 *      f | g | b
 *         ---
 *      e |   | c
 *         ---
 *          d
 *
 *  In the segment translation table (seg7[]), the segments are given
 *  in the order G == bit 0x40 through A == bit 0x01.
 *
 *  Once the appropriate segment on/off bitpattern from seg7[] is obtained,
 *  we need to look into the segments[] array to find the drawing coordinates
 *  for the individual segments.  We've decided on a "diamond" shaped
 *  7-segment pattern for aesthetical reasons, so the segments[] array
 *  is a structure containing a set of vectors:
 *
 *      ss, se      short start and end
 *      ls, le      long start and end
 *      p1, p2, p3  three points
 *
 *  To illustrate, consider the "a" segment (identical to the "g" and "d"):
 *
 *       /=================\
 *      /                   \
 *      \                   /
 *       \=================/
 *
 *  The "short" start and end coordinates refer to the "X" coordinates of the
 *  top and bottom line, whereas the "long" start and end coordinates refer
 *  to the "X" coordinates of the middle -- the long part.
 *  The p1, p2, and p3 values are the "Y" coordinate values of the top, middle,
 *  and bottom line segments.
 *  A similar naming convention applies for vertical segments, except that the
 *  "short" and "long" refer to the "Y" axis and the p1, p2, and p2 refer to
 *  the "X" axis.
 *
 *  The segments[] array structure also contains an orientation for the
 *  diamond, 'h' for horizontal and 'v' for vertical.
 *  The diamonds are drawn in two pieces.  For horizontal segments:
 *
 *      1)  [(ss, p1) - (se, p1)] - [(ls, p2) - (le, p2)]
 *      2)  [(ls, p2) - (le, p2)] - [(ss, p3) - (se, p3)]
 *
 *  and for vertical segments:
 *
 *      1)  [(p1, ss) - (p1, se)] - [(p2, ls) - (p2, le)]
 *      2)  [(p2, ls) - (p2, le)] - [(p3, ss) - (p2, se)]
 *
 *  This effectively forms the two halves (marked "1" and "2" above) of the
 *  diamond.  The original diamond pattern was drawn on a piece of graph
 *  paper, hence the "funky" 13 x 18 dimensions :-)
 *
 *  The bitmap into which the characters are drawn is assumed to be 1 byte
 *  per pixel, in row (X) order.
*/
 
static char seg7 [16] =
{
            //  -gfe dcba
    0x3f,   //  0011 1111 (0)
    0x06,   //  0000 0110 (1)
    0x5b,   //  0101 1011 (2)
    0x4f,   //  0100 1111 (3)
    0x66,   //  0110 0110 (4)
    0x6d,   //  0110 1101 (5)
    0x7d,   //  0111 1101 (6)
    0x07,   //  0000 0111 (7)
    0x7f,   //  0111 1111 (8)
    0x6f,   //  0110 1111 (9)
    0x77,   //  0111 0111 (a)
    0x7c,   //  0111 1100 (b)
    0x58,   //  0101 1000 (c)
    0x5e,   //  0101 1110 (d)
    0x79,   //  0111 1001 (e)
    0x71    //  0111 0001 (f)
};

typedef struct {
    int     orientation;

    double  ss, se, ls, le;         // short start and end, long start and end
    double  p1, p2, p3;             // three points
}   xy_t;

// these are the vectors for the segments; they are scaled y {0..18} and x {0..13}
xy_t segments [7] =
{
    {'h',  3,  8,  2,  9,  1,  2,  3},      // a
    {'v',  3,  8,  2,  9,  8,  9, 10},      // b
    {'v', 10, 15,  9, 16,  8,  9, 10},      // c
    {'h',  3,  8,  2,  9, 15, 16, 17},      // d
    {'v', 10, 15,  9, 16,  1,  2,  3},      // e
    {'v',  3,  8,  2,  9,  1,  2,  3},      // f
    {'h',  3,  8,  2,  9,  8,  9, 10}       // g
};

static int bits [8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void
render_7segment (char *digits, unsigned char *r, int xsize, int ysize)
{
    int     xo;
    double  xm, ym;
    int     ndigits;
    int     d;
    int     segs;
    int     seg;
    int     x, y;
    double  z;
    int     ss, se, ls, le, p1, p2, p3;

    ndigits = strlen (digits);
    xm = (double) xsize / (double) ndigits / 13.;
    ym = (double) ysize / 18.;

    for (d = 0; digits [d]; d++) {
        xo = xm * 13. * d;
        if (isxdigit (digits [d])) {
            segs = seg7 [isdigit (digits [d]) ? digits [d] - '0' : tolower (digits [d]) - 'a' + 10];
        } else {
            segs = 0;           // anything else is a blank
        }
        for (seg = 0; seg < 8; seg++) {
            if (segs & bits [seg]) {    // need to render it
                if (segments [seg].orientation == 'h') {
                    ss = segments [seg].ss * xm;
                    se = segments [seg].se * xm;
                    ls = segments [seg].ls * xm;
                    le = segments [seg].le * xm;
                    p1 = segments [seg].p1 * ym;
                    p2 = segments [seg].p2 * ym;
                    p3 = segments [seg].p3 * ym;

                    for (y = p1; y <= p2; y++) {
                        z = ((double) y - p1) / (p2 - p1);
                        for (x = ss + z * (ls - ss); x <= se + z * (le - se); x++) {
                            r [x + xo + y * xsize] = 1;
                        }
                    }

                    for (y = p2; y <= p3; y++) {
                        z = ((double) y - p2) / (p3 - p2);
                        for (x = ls + z * (ss - ls); x <= le + z * (se - le); x++) {
                            r [x + xo + y * xsize] = 1;
                        }
                    }

                } else {

                    ss = segments [seg].ss * ym;
                    se = segments [seg].se * ym;
                    ls = segments [seg].ls * ym;
                    le = segments [seg].le * ym;
                    p1 = segments [seg].p1 * xm;
                    p2 = segments [seg].p2 * xm;
                    p3 = segments [seg].p3 * xm;
    
                    for (x = p1; x <= p2; x++) {
                        z = ((double) x - p1) / (p2 - p1);
                        for (y = ss + z * (ls - ss); y <= se + z * (le - se); y++) {
                            r [x + xo + y * xsize] = 1;
                        }
                    }

                    for (x = p2; x <= p3; x++) {
                        z = ((double) x - p2) / (p3 - p2);
                        for (y = ls + z * (ss - ls); y <= le + z * (se - le); y++) {
                            r [x + xo + y * xsize] = 1;
                        }
                    }
                }
            }
        }
    }
}

