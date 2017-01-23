
#
#	makefile for webcounter phase III
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
#	2003 08 12	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = main.o gif.o 7seg.o 8x8.o
CFLAGS = -w9 $(DEBUG) -Wno-parentheses
LDFLAGS = $(DEBUG)

Common = Makefile gif.h 7seg.h 8x8.h

# ID Section
ID_Name = "-iNAME=webcounter3"
ID_Desc = "-iDESCRIPTION=Web Counter Phase III"
ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID = $(ID_Name) $(ID_Desc) $(ID_Copy) $(ID_Vers)

webcounter3: $(OBJECTS) main.use
	cc $(CFLAGS) -o webcounter3 $(OBJECTS) version.c $(LDFLAGS)
	usemsg $(ID) webcounter3 main.use

main.o: main.c $(Common)

gif.o: gif.c $(Common)

7seg.o: 7seg.c $(Common)

8x8.o: 8x8.c $(Common)

clean:
	rm -f *.o webcounter3

install:
	cp webcounter3 /usr/local/bin/webcounter3

release:
	make clean
	rm -f webcounter3.tar*
	tar cvf webcounter3.tar *.[ch] Makefile
	gzip -9v webcounter3.tar
