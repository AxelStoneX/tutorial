
#
#	makefile for ramdisk
#
#  Copyright 2002, 2003 QNX Software Systems Limited. All Rights Reserved.
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
#	2002 08 13	R. Krten			created
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = attr.o dirent.o \
		  c_link.o c_mknod.o c_mount.o c_open.o  \
		  c_readlink.o c_rename.o c_unlink.o \
		  debug.o \
		  io_read.o \
		  io_write.o \
		  main.o \
		  pathname.o \
		  ramdisk_io_read.o ramdisk_io_write.o \
		  utils.o
		  
CFLAGS = -w9 $(DEBUG) -Wno-parentheses
LDFLAGS = $(DEBUG) -static

Common = Makefile cfs.h

CFSLIB = ../lib/cfs

# ID Section
ID_Name = "-iNAME=ramdisk"
ID_Desc = "-iDESCRIPTION=RAM-disk"
ID_Copy = "-iCOPYRIGHT=Copyright 2002, 2003 QNX Software Systems Limited. All Rights Reserved."
ID_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID = $(ID_Name) $(ID_Desc) $(ID_Copy) $(ID_Vers)

ramdisk : $(OBJECTS) main.use $(CFSLIB).a
	cc $(CFLAGS) -o ramdisk $(OBJECTS) version.c $(LDFLAGS) $(CFSLIB).a
	usemsg $(ID) ramdisk main.use

attr.o: attr.c $(Common)

c_link.o: c_link.c $(Common)

c_mknod.o: c_mknod.c $(Common)

c_mount.o: c_mount.c $(Common)

c_open.o: c_open.c $(Common)

c_readlink.o: c_readlink.c $(Common)

c_rename.o: c_rename.c $(Common)

c_unlink.o: c_unlink.c $(Common)

debug.o: debug.c $(Common)

dirent.o: dirent.c $(Common)

io_read.o: io_read.c $(Common)

io_write.o: io_write.c $(Common)

main.o: main.c $(Common)

pathname.o: pathname.c $(Common)

ramdisk_io_read.o: ramdisk_io_read.c $(Common)

ramdisk_io_write.o: ramdisk_io_write.c $(Common)

utils.o: utils.c $(Common)

clean:
	rm -f *.o ramdisk

install:
	cp ramdisk /usr/local/bin/ramdisk

release:
	make clean ; make
	rm -f ramdisk.tar*
	tar cvf ramdisk.tar Makefile README COVERAGE *.c *.h *.use ramdisk
	gzip -9v ramdisk.tar

