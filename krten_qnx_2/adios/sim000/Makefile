
#
#	makefile for SIM-000 driver
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
#	2003 10 09	R. Krten			created; needed samples for book but didn't have boards any more :-)
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = main.o sim000.o iofuncs.o
LIBRARIES = ../lib/adioslib.a
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../lib/
LDFLAGS = $(DEBUG)

Common = Makefile

# ID Section
ID_Name = "-iNAME=sim000"
ID_Desc = "-iDESCRIPTION=SIM-000 Driver"
ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID = $(ID_Name) $(ID_Desc) $(ID_Copy) $(ID_Vers)

sim000: $(OBJECTS) main.use ../lib/adioslib.a
	cc $(CFLAGS) -o sim000 $(OBJECTS) $(LIBRARIES) version.c $(LDFLAGS)
	usemsg $(ID) sim000 main.use

main.o: main.c $(Common)

iofuncs.o: iofuncs.c $(Common)

sim000.o: sim000.c $(Common)

clean:
	rm -f *.o sim000

install:
	cp sim000 /usr/local/bin/sim000
