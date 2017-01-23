
/*
 *  tarfs.h
 *
 *  Copyright 2003 QNX Software Systems Limited. All Rights Reserved.
 * 
 *  You must obtain a written license from and pay applicable license fees to QNX 
 *  Software Systems Limited before you may reproduce, modify or distribute this software, 
 *  or any work that includes all or part of this software.   Free development 
 *  licenses are available for evaluation and non-commercial purposes.  For more 
 *  information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 *  This file may contain contributions from others.  Please review this entire 
 *  file for other proprietary rights or license notices, as well as the QNX 
 *  Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 *  for other information.
 *
 *  This module contains the manifest constants and declarations for
 *  the TARFS
 *
 *  2003 01 03  R. Krten        created
 *  2003 01 12  R. Krten        copied from ramdisk for tarfs
 *  2003 10 23  R. Krten        released for publication
*/

/*
 *  MANIFEST CONSTANTS
*/

#define BLOCKSIZE           4096                // (MUST BE 2^N!) the size of the blocks used in the filesystem

int             tarfs_io_read_dir (resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int             tarfs_io_read_file (resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int             analyze_tar_file (cfs_attr_t *a, char *fname);

