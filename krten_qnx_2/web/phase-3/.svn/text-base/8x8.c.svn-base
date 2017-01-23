
/*
 *  8x8.c
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
 *  This module contains a 8 by 8 "dot" font generator.
 *
 *  2003 04 23  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "8x8.h"

/*
 *  8x8 dot generator
 *
 *  This render module presents a simple 8x8 font for the digits
 *  0 through 9.
 *
 *  The digit font table is bit-oriented, with each byte carrying the
 *  information for a row.  There are, therefore, 8 bytes per digit,
 *  one for each row.
 *
 *  Rendering consists of drawing or not drawing a filled square as
 *  appropriate for the bit position.
*/

static unsigned char font [11][8] =
{
    {
        0x3c,       // ..####..
        0x42,       // .#....#.
        0xa1,       // #.#....#
        0x91,       // #..#...#
        0x89,       // #...#..#
        0x85,       // #....#.#
        0x42,       // .#....#.
        0x3c        // ..####..
    }, {
        0x08,       // ....#...
        0x18,       // ...##...
        0x08,       // ....#...
        0x08,       // ....#...
        0x08,       // ....#...
        0x08,       // ....#...
        0x08,       // ....#...
        0x7e        // .######.
    }, {
        0x3c,       // ..####..
        0x42,       // .#....#.
        0x81,       // #......#
        0x01,       // .......#
        0x06,       // .....##.
        0x18,       // ...##...
        0x60,       // .##.....
        0xff        // ########
    }, {
        0xff,       // ########
        0x03,       // ......##
        0x06,       // .....##.
        0x0c,       // ....##..
        0x06,       // .....##.
        0xc3,       // ##....##
        0x63,       // .##...##
        0x3e        // ..#####.
    }, {
        0x0c,       // ....##..
        0x34,       // ..##.#..
        0x64,       // .##..#..
        0xc4,       // ##...#..
        0xff,       // ########
        0x04,       // .....#..
        0x04,       // .....#..
        0x04        // .....#..
    }, {
        0xff,       // ########
        0x80,       // #.......
        0x80,       // #.......
        0xfc,       // ######..
        0x02,       // ......#.
        0x01,       // .......#
        0x82,       // #.....#.
        0x7c        // .#####..
    }, {
        0x3c,       // ..####..
        0x42,       // .#....#.
        0x80,       // #.......
        0xfc,       // ######..
        0x82,       // #.....#.
        0x81,       // #......#
        0x42,       // .#....#.
        0x3c        // ..####..
    }, {
        0xff,       // ########
        0x03,       // ......##
        0x03,       // ......##
        0x06,       // .....##.
        0x0c,       // ....##..
        0x18,       // ...##...
        0x18,       // ...##...
        0x18        // ...##...
    }, {
        0x3c,       // ..####..
        0x42,       // .#....#.
        0x42,       // .#....#.
        0x3c,       // ..####..
        0x42,       // .#....#.
        0x81,       // #......#
        0x42,       // .#....#.
        0x3c,       // ..####..
    }, {
        0x3c,       // ..####..
        0x42,       // .#....#.
        0x81,       // #......#
        0x41,       // .#.....#
        0x3f,       // ..######
        0x01,       // .......#
        0x42,       // .#....#.
        0x3c        // ..####..
    }, {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // blank
    }
};


static int bits [8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

void
render_8x8 (char *digits, unsigned char *r, int xsize, int ysize)
{
    int     xsq, ysq;       // size of each X and Y square
    int     x1, y1;
    int     d;
    int     row, col;
    int     ndigits;
    unsigned char *f;       // font pointer;

    ndigits = strlen (digits);
    xsq = (double) xsize / (double) ndigits / 9.;
    ysq = (double) ysize / 9.;
    
    for (d = 0; digits [d]; d++) {
        if (isdigit (digits [d])) {
            f = font [digits [d] - '0'];
        } else {
            f = font [10];  // font 10 is the blank font
        }

        for (row = 0; row < 8; row++) {
            for (col = 0; col < 8; col++) {
                if (f [row] & bits [col]) {
                    for (x1 = 0; x1 <= xsq; x1++) {
                        for (y1 = 0; y1 <= ysq; y1++) {
                            r [d * xsq * 9 + x1 + col * xsq + xsize * (y1 + row * ysq)] = 1;
                        }
                    }
                }
            }
        }
    }
}

