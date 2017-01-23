
/*
 *  iofuncs.h
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
 *  the I/O and Connect functions for the resource manager.
 *
 *  2003 10 23  R. Krten        released for publication
*/

#include <devctl.h>

#ifdef  USING_AN_EXTENDED_ATTRIBUTES_STRUCTURE
struct my_attr_s;
#define IOFUNC_ATTR_T struct my_attr_s
#endif  // USING_AN_EXTENDED_ATTRIBUTES_STRUCTURE

#ifdef  USING_AN_EXTENDED_OCB_STRUCTURE
struct my_ocb_s;
#define IOFUNC_OCB_T struct my_ocb_s
#endif  // USING_AN_EXTENDED_OCB_STRUCTURE

#include <sys/iofunc.h>

#ifdef  USING_AN_EXTENDED_ATTRIBUTES_STRUCTURE
typedef struct my_attr_s
{
    iofunc_attr_t       attr;
    int                 my_stuff;
}   my_attr_t;
#endif  // USING_AN_EXTENDED_ATTRIBUTES_STRUCTURE

#ifdef  USING_AN_EXTENDED_OCB_STRUCTURE
typedef struct my_ocb_s
{
    iofunc_ocb_t        ocb;
    int                 my_stuff;
}   my_ocb_t;
#endif  // USING_AN_EXTENDED_OCB_STRUCTURE

#include <sys/resmgr.h>
#include <sys/iofunc.h>

