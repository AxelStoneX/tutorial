
#
#	makefile for multiple executables
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
#	2003 06 24	R. Krten			created (with "showsamp")
#	2003 07 10	R. Krten			added "tag"
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../lib/ -I../api/
LDFLAGS = $(DEBUG)

#	List executables here

Executables = showsamp tag

Common = Makefile ../api/api.h

#
#	Common section (common to all executables)
#

all: $(Executables)

## #
## #	TEMPLATE, duplicate per-executable
## #
## 
## MODULE_OBJECTS = module.o
## 
## # ID Section
## MODULE_ID_Name = "-iNAME=MODULE"
## MODULE_ID_Desc = "-iDESCRIPTION=DESCRIPTION OF MODULE HERE"
## MODULE_ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
## MODULE_ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" module_version.c`"
## MODULE_ID = $(MODULE_ID_Name) $(MODULE_ID_Desc) $(MODULE_ID_Copy) $(MODULE_ID_Vers)
## 
## module: $(MODULE_OBJECTS) module.use
## 	cc $(CFLAGS) -o module $(MODULE_OBJECTS) module_version.c $(LDFLAGS)
## 	usemsg $(MODULE_ID) module module.use
## 
## module.o: module.c $(Common)

#
#	showsamp
#

SHOWSAMP_OBJECTS = showsamp.o

# ID Section
SHOWSAMP_ID_Name = "-iNAME=showsamp"
SHOWSAMP_ID_Desc = "-iDESCRIPTION=Utility to display shared memory region maintained by ADIOS"
SHOWSAMP_ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
SHOWSAMP_ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" showsamp_version.c`"
SHOWSAMP_ID = $(SHOWSAMP_ID_Name) $(SHOWSAMP_ID_Desc) $(SHOWSAMP_ID_Copy) $(SHOWSAMP_ID_Vers)

showsamp: $(SHOWSAMP_OBJECTS) showsamp.use
	cc $(CFLAGS) -o showsamp $(SHOWSAMP_OBJECTS) showsamp_version.c $(LDFLAGS)
	usemsg $(SHOWSAMP_ID) showsamp showsamp.use

showsamp.o: showsamp.c $(Common)

#
#	tag
#

TAG_OBJECTS = tag.o

# ID Section
TAG_ID_Name = "-iNAME=tag"
TAG_ID_Desc = "-iDESCRIPTION=Utility to show, monitor, and set tags"
TAG_ID_Copy = "-iCOPYRIGHT=(C) Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
TAG_ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" tag_version.c`"
TAG_ID = $(TAG_ID_Name) $(TAG_ID_Desc) $(TAG_ID_Copy) $(TAG_ID_Vers)

tag: $(TAG_OBJECTS) tag.use ../lib/adioslib.a
	cc $(CFLAGS) -o tag $(TAG_OBJECTS) tag_version.c $(LDFLAGS) ../lib/adioslib.a
	usemsg $(TAG_ID) tag tag.use

tag.o: tag.c $(Common)

#
#	Common section (common to all executables)
#

clean:
	rm -f *.o $(Executables)

install:
	cp $(Executables) /usr/local/bin/
