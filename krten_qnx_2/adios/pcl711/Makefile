
#
#	makefile for PCL-711 driver
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
#	2003 06 17	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = main.o pcl711.o iofuncs.o
LIBRARIES = ../lib/adioslib.a
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../lib/
LDFLAGS = $(DEBUG)

Common = Makefile

# ID Section
ID_Name = "-iNAME=pcl711"
ID_Desc = "-iDESCRIPTION=PCL-711 Driver"
ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID = $(ID_Name) $(ID_Desc) $(ID_Copy) $(ID_Vers)

pcl711: $(OBJECTS) main.use ../lib/adioslib.a
	cc $(CFLAGS) -o pcl711 $(OBJECTS) $(LIBRARIES) version.c $(LDFLAGS)
	usemsg $(ID) pcl711 main.use

main.o: main.c $(Common)

iofuncs.o: iofuncs.c $(Common)

pcl711.o: pcl711.c $(Common)

clean:
	rm -f *.o pcl711

install:
	cp pcl711 /usr/local/bin/pcl711
