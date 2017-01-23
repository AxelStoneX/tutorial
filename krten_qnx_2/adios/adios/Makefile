#
#	makefile for ADIOS
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
#	2003 06 19	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = main.o iofuncs.o adios.o daq.o
LIBRARIES = ../lib/adioslib.a
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../api/ -I../lib/
LDFLAGS = $(DEBUG)

Common = Makefile ../api/api.h ../lib/parser.h

# ID Section
ID_Name = "-iNAME=ADIOS"
ID_Desc = "-iDESCRIPTION=Analog / Digital I/O Server (ADIOS)"
ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID = $(ID_Name) $(ID_Desc) $(ID_Copy) $(ID_Vers)

adios: $(OBJECTS) main.use ../lib/adioslib.a
	cc $(CFLAGS) -o adios $(OBJECTS) $(LIBRARIES) version.c $(LDFLAGS)
	usemsg $(ID) adios main.use

main.o: main.c $(Common)

iofuncs.o: iofuncs.c $(Common)

adios.o: adios.c $(Common)

daq.o: daq.c $(Common)

clean:
	rm -f *.o adios

install:
	cp adios /usr/local/bin/adios
