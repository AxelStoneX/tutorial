
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
#	2003 08 27	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
CFLAGS = -w9 $(DEBUG) -Wno-parentheses
LDFLAGS = $(DEBUG)

#	List executables here

Executables = procfs

Common = Makefile

#
#	Common section (common to all executables)
#

all: $(Executables)

#
#	TEMPLATE, duplicate per-executable
#

PROCFS_OBJECTS = procfs.o

# ID Section
PROCFS_ID_Name = "-iNAME=procfs"
PROCFS_ID_Desc = "-iDESCRIPTION=/proc filesystem dumper"
PROCFS_ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
PROCFS_ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" procfs_version.c`"
PROCFS_ID = $(PROCFS_ID_Name) $(PROCFS_ID_Desc) $(PROCFS_ID_Copy) $(PROCFS_ID_Vers)

procfs: $(PROCFS_OBJECTS) procfs.use
	cc $(CFLAGS) -o procfs $(PROCFS_OBJECTS) procfs_version.c $(LDFLAGS)
	usemsg $(PROCFS_ID) procfs procfs.use

procfs.o: procfs.c $(Common)

### #
### #	TEMPLATE, duplicate per-executable
### #
### 
### MODULE_OBJECTS = module.o
### 
### # ID Section
### MODULE_ID_Name = "-iNAME=MODULE"
### MODULE_ID_Desc = "-iDESCRIPTION=DESCRIPTION OF MODULE HERE"
### MODULE_ID_Copy = "-iCOPYRIGHT=Copyright 2003 QNX Software Systems Limited. All Rights Reserved."
### MODULE_ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" module_version.c`"
### MODULE_ID = $(MODULE_ID_Name) $(MODULE_ID_Desc) $(MODULE_ID_Copy) $(MODULE_ID_Vers)
### 
### module: $(MODULE_OBJECTS) module.use
### 	cc $(CFLAGS) -o module $(MODULE_OBJECTS) module_version.c $(LDFLAGS)
### 	usemsg $(MODULE_ID) module module.use
### 
### module.o: module.c $(Common)

#
#	Common section (common to all executables)
#

clean:
	rm -f *.o $(Executables)

install:
	cp $(Executables) /usr/local/bin/
