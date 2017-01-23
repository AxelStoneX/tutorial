
#
#	makefile for DIO-144 driver
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
#	2003 07 03	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = main.o dio144.o iofuncs.o
LIBRARIES = ../lib/adioslib.a
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../lib/
LDFLAGS = $(DEBUG)

Common = Makefile

# ID Section
ID_Name = "-iNAME=dio144"
ID_Desc = "-iDESCRIPTION=DIO-144 Driver"
ID_Copy = "-iCOPYRIGHT=(C) Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID = $(ID_Name) $(ID_Desc) $(ID_Copy) $(ID_Vers)

dio144: $(OBJECTS) main.use ../lib/adioslib.a
	cc $(CFLAGS) -o dio144 $(OBJECTS) $(LIBRARIES) version.c $(LDFLAGS)
	usemsg $(ID) dio144 main.use

main.o: main.c $(Common)

iofuncs.o: iofuncs.c $(Common)

dio144.o: dio144.c $(Common)

clean:
	rm -f *.o dio144

install:
	cp dio144 /usr/local/bin/dio144
