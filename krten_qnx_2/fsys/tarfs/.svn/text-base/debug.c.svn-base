
/*
 *  debug.c
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
 *  This module contains debug helpers.
 *
 *  2002 08 24  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

extern  char    *progname;                  // main.c
extern  char    *version;                   // version.c

extern  int     optd;
extern  int     optv;                       // main.c

void *
debug_malloc (const char *f, int l, int s)
{
    void    *p;

    p = malloc (s);
    if (optd) {
        printf ("%p MALLOC %d at %s %d\n", p, s, f, l);
    }
    return (p);
}

void *
debug_calloc (const char *f, int l, int s, int c)
{
    void    *p;

    p = calloc (s, c);
    if (optd) {
        printf ("%p CALLOC %d, %d at %s %d\n", p, s, c, f, l);
    }
    return (p);
}

void *
debug_realloc (const char *f, int l, void *p, int s)
{
    void    *n;

    n = realloc (p, s);
    if (optd) {
        printf ("%p REALLOC %p %d at %s %d\n", n, p, s, f, l);
    }
    return (n);
}

char *
debug_strdup (const char *f, int l, const char *s)
{
    char    *n;

    n = strdup (s);
    if (optd) {
        printf ("%p STRDUP (%s) at %s %d\n", n, s, f, l);
    }
    return (n);
}

void
debug_free (const char *f, int l, void *p)
{
    if (optd) {
        printf ("%p FREE at %s %d\n", p, f, l);
    }
    free (p);
}

