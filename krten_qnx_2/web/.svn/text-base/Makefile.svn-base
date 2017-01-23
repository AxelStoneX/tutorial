
#
#	makefile for all webcounter phases
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



all:
	cd phase-1 ; make
	cd phase-2 ; make
	cd phase-3 ; make

clean:
	cd phase-1 ; make clean
	cd phase-2 ; make clean
	cd phase-3 ; make clean

install:
	make
	cp phase-1/webcounter1 /usr/local/bin/webcounter1
	cp phase-2/webcounter2 /usr/local/bin/webcounter2
	cp phase-3/webcounter3 /usr/local/bin/webcounter3
