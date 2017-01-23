
#
#	makefile for tarfs and its little helper, mount_tarfs
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
		  tarfs.o tarfs_io_read.o \
		  utils.o

M_OBJECTS = m_main.o
		  
CFLAGS = -w9 $(DEBUG) -Wno-parentheses -I../lib
LDFLAGS = $(DEBUG)

Common = Makefile cfs.h

CFSLIB = ../lib/cfs
all: tarfs mount_tarfs $(CFSLIB).a

# ID Section
ID1_Name = "-iNAME=tarfs"
ID1_Desc = "-iDESCRIPTION=.tar filesystem"
ID1_Copy = "-iCOPYRIGHT=Copyright 2002, 2003 QNX Software Systems Limited. All Rights Reserved."
ID1_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" version.c`"
ID1 = $(ID1_Name) $(ID1_Desc) $(ID1_Copy) $(ID1_Vers)

ID2_Name = "-iNAME=mount_tarfs"
ID2_Desc = "-iDESCRIPTION=.tar filesystem mount helper"
ID2_Copy = "-iCOPYRIGHT=Copyright 2002, 2003 QNX Software Systems Limited. All Rights Reserved."
ID2_Vers = "-iVERSION=`sed \"1s/.*\\"\\(.*\\)\\";/\\1/\" m_version.c`"
ID2 = $(ID2_Name) $(ID2_Desc) $(ID2_Copy) $(ID2_Vers)

tarfs: $(OBJECTS) main.use $(CFSLIB).a
	cc $(CFLAGS) -o tarfs -lz $(OBJECTS) version.c $(LDFLAGS) $(CFSLIB).a
	usemsg $(ID1) tarfs main.use

mount_tarfs: $(M_OBJECTS) m_main.use
	cc $(CFLAGS) -o mount_tarfs $(M_OBJECTS) m_version.c $(LDFLAGS)
	usemsg $(ID2) mount_tarfs m_main.use

m_main.o: m_main.c

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

tarfs.o: tarfs.c $(Common)

tarfs_io_read.o: tarfs_io_read.c $(Common)

utils.o: utils.c $(Common)

clean:
	rm -f *.o tarfs mount_tarfs

install:
	cp tarfs /usr/local/bin/tarfs
	cp mount_tarfs /usr/local/bin/mount_tarfs

release:
	make clean ; make
	rm -f tarfs.tar*
	tar cvf tarfs.tar Makefile README *.c *.h *.use tarfs mount_tarfs
	gzip -9v tarfs.tar

