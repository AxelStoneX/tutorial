
/*
 *  utils.c
 *
 *  Copyright 2002, 2003, 2012 QNX Software Systems Limited. All Rights Reserved.
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
 *  This module is the RAMDISK's utility functions.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <devctl.h>
#include <limits.h>

// these are required for dcmd_blk.h
#include <sys/types.h>
#include <stdint.h>
#include <sys/disk.h>
#include <sys/fs_stats.h>
#include <sys/dcmd_blk.h>

#include "cfs.h"

/*
 *  Debug, diags, etc.
*/

void
dump_ioflag (int ioflag)
{
#ifdef VDebug
if (optd > 1) {
    printf ("%cR%cW ", ioflag & 0x01 ? '+' : '-', ioflag & 0x02 ? '+' : '-');

    if (ioflag & O_NONBLOCK)    printf ("O_NONBLOCK ");
    if (ioflag & O_APPEND)      printf ("O_APPEND ");
    if (ioflag & O_DSYNC)       printf ("O_DSYNC ");
    if (ioflag & O_RSYNC)       printf ("O_RSYNC ");
    if (ioflag & O_SYNC)        printf ("O_SYNC ");
    if (ioflag & O_CREAT)       printf ("O_CREAT ");
    if (ioflag & O_TRUNC)       printf ("O_TRUNC ");
    if (ioflag & O_EXCL)        printf ("O_EXCL ");
    if (ioflag & O_NOCTTY)      printf ("O_NOCTTY ");
    if (ioflag & O_CLOEXEC)     printf ("O_CLOEXEC ");
    if (ioflag & O_REALIDS)     printf ("O_REALIDS ");
    if (ioflag & O_LARGEFILE)   printf ("O_LARGEFILE ");
    if (ioflag & O_ASYNC)       printf ("O_ASYNC ");

    printf ("\n");
}
#endif  // VDebug
}

void
dump_c_connect (struct _io_connect *c)
{
#ifdef  VDebug
if (optd > 1) {
    printf ("Dump of _IO_CONNECT message at %p\n", c);
    printf ("\ttype         %04X\n", c -> type);
    printf ("\tsubtype      %04X\n", c -> subtype);
    printf ("\tfile_type    %08X\n", c -> file_type);
    printf ("\treply_max    %04X\n", c -> reply_max);
    printf ("\tentry_max    %04X\n", c -> entry_max);
    printf ("\tkey          %08X\n", c -> key);
    printf ("\thandle       %08X\n", c -> handle);
    printf ("\tioflag       %08X\n", c -> ioflag);
    printf ("\tmode         %08X (0%o) ", c -> mode, c -> mode);
        switch (c -> mode & 0xF000) {
        case    _S_IFMT:    printf ("_S_IFMT ");    break;
        case    _S_IFIFO:   printf ("_S_IFIFO ");   break;
        case    _S_IFCHR:   printf ("_S_IFCHR ");   break;
        case    _S_IFDIR:   printf ("_S_IFDIR ");   break;
        case    _S_IFNAM:   printf ("_S_IFNAM ");   break;
        case    _S_IFBLK:   printf ("_S_IFBLK ");   break;
        case    _S_IFREG:   printf ("_S_IFREG ");   break;
        case    _S_IFLNK:   printf ("_S_IFLNK ");   break;
        case    _S_IFSOCK:  printf ("_S_IFSOCK ");  break;
        }
        printf ("\n");

    printf ("\tsflag        %04X\n", c -> sflag);
    printf ("\taccess       %04X\n", c -> access);
    printf ("\tzero         %04X\n", c -> zero);
    printf ("\tpath_len     %04X\n", c -> path_len);
    printf ("\teflag        %02X\n", c -> eflag);
    printf ("\textra_type   %02X\n", c -> extra_type);
    printf ("\textra_len    %04X\n", c -> extra_len);
    printf ("\tpath         %s\n", c -> path);
}
#endif  // VDebug
}

