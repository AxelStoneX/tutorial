
/*
 *  io_space.c
 *
 *  Copyright 2002, 2003 QNX Software Systems Limited. All Rights Reserved.
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
 *  This module is the CAPTIVE FILESYSTEM utility library's io_space handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2003 10 23  R. Krten        released for publication
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

#include "cfs.h"

/*
 *  message:
 *  struct _io_space {
 *      _Uint16t                    type;
 *      _Uint16t                    combine_len;
 *      _Uint16t                    subtype;            // F_ALLOCSP or F_FREESP
 *      short                       whence;
 *      _Uint64t                    start;
 *      _Uint64t                    len;                // zero means to end of file
 *  };
 *
 *  F_ALLOCSP 
 *      If the size of the file is less than the number of bytes specified by 
 *      the extra arg argument, the file size is extended with NUL characters. 
 *  F_FREESP 
 *      Truncate the file to the size (in bytes) specified by the extra 
 *      argument, arg. 
*/

int
cfs_io_space (resmgr_context_t *ctp, io_space_t *msg, RESMGR_OCB_T *ocb)
{
    int     sts;

D   printf ("%s %d:  cfs_io_space () subtype %d whence %d start %lld len %lld\n", __FILE__, __LINE__, msg -> i.subtype, msg -> i.whence, msg -> i.start, msg -> i.len);
    sts = iofunc_space_verify (ctp, msg, ocb, NULL);
    if (sts != EOK) {
D       printf ("%s %d:  cfs_io_space called iofunc_space_verify and it returned %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
        return (sts);
    }

    switch (msg -> i.subtype) {
    case    F_ALLOCSP:
D       printf ("%s %d:  cfs_io_space () subtype F_ALLOCSP\n", __FILE__, __LINE__);
        break;
    case    F_ALLOCSP64:
D       printf ("%s %d:  cfs_io_space () subtype F_ALLOCSP64\n", __FILE__, __LINE__);
        break;
    case    F_FREESP:
D       printf ("%s %d:  cfs_io_space () subtype F_FREESP\n", __FILE__, __LINE__);
        break;
    case    F_FREESP64:
D       printf ("%s %d:  cfs_io_space () subtype F_FREESP64\n", __FILE__, __LINE__);
        break;
    default:
D       printf ("%s %d:  cfs_io_space () subtype %d unknown -> EINVAL\n", __FILE__, __LINE__, msg -> i.subtype);
        return (EINVAL);
    }
    return (_RESMGR_DEFAULT);
}

