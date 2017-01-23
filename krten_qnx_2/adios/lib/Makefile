
#
#	makefile for ADIOS and Driver Library
#
#  Copyright 2003 QNX Software Systems Limited. All Rights Reserved.
# 
#  You must obtain a written license from and pay applicable license fees to QNX 
#  Software Systems Limited before you may reproduce, modify or distribute this software, 
#  or any work that includes all or part of this software.   Free development 
#  licenses are available for evaluation and non-commercial purposes.  For more 
#  information visit http://licensing.qnx.com or email licensing@qnx.com.
#  
#  This file may contain contributions from others.  Please review this entire 
#  file for other proprietary rights or license notices, as well as the QNX 
#  Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
#  for other information.
#
#	2003 07 07	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = parser.o client.o
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../api/
LDFLAGS = $(DEBUG)

Common = Makefile

adioslib.a: $(OBJECTS)
	cc $(CFLAGS) -A adioslib.a $(OBJECTS) $(LDFLAGS)

parser.o: parser.c $(Common)

client.o: client.c $(Common)

clean:
	rm -f *.o adioslib.a

install:
	cp adioslib.a /usr/local/lib/adioslib.a
