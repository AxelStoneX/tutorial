
#
#	makefile for CFS library
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
#	2003 02 03	R. Krten			created library out of CFS components in TARFS and RAMDISK
#	2003 10 23	R. Krten			released for publication
#

DEBUG = -g2
OBJECTS = \
	block.o\
	cfs_attr_init.o\
	cfs_func_init.o\
	io_close.o\
	io_devctl.o\
	io_space.o\
	mpool.o\


CFLAGS = -w9 $(DEBUG) -Wno-parentheses
LDFLAGS = $(DEBUG)

Common = Makefile

cfs.a: $(OBJECTS)
	cc $(CFLAGS) -A cfs $(OBJECTS) $(LDFLAGS)

block.o: block.c $(Common)

cfs_attr_init.o: cfs_attr_init.c $(Common)

cfs_func_init.o: cfs_func_init.c $(Common)

io_close.o: io_close.c $(Common)

io_devctl.o: io_devctl.c $(Common)

io_space.o: io_space.c $(Common)

mpool.o: mpool.c $(Common)

clean:
	rm -f *.o cfs.a

install:
	cp cfs /usr/local/lib/cfs
