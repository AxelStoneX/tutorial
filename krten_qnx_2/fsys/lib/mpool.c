
/*
 *  mpool.c
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
 *  This module contains the memory pool library.  It is thread safe.
 *  See "mpool.h" for documentation.
 *
 *  2002 11 24  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include "cfs.h"

extern  char    *progname;                                                      // main.c
extern  char    *version;                                                       // version.c

extern  int     optv;                                                           // main.c

/*
 *  mpool_create
 *  mpool_destroy
*/

mpool_t *
mpool_create (size_t nbytes, uint32_t nels, uint32_t flags, const char *name)
{
    mpool_t     *poolid;
    uint32_t    i;
    pool_list_t *ptrlist;                                                       // an alias for "ptr"
    char        *charptr;                                                       // and another for doing pointer math

D   printf ("%s %d:  mpool_create (nbytes %d nels %d flags 0x%08x name \"%s\")\n", __FILE__, __LINE__, nbytes, nels, flags, name ? name : "<no name given>");

    /*
     *  create the pool handle
    */

    poolid = calloc (sizeof (*poolid), 1);
    if (poolid == NULL) {
D       printf ("%s %d:  mpool_create couldn't calloc %dx1 bytes\n", __FILE__, __LINE__, sizeof (*poolid));
        return (NULL);
    }

    /*
     *  fixup size (since we need the region to at least contain a pool_list_t,
     *  it should be at least that big)
    */

    if (nbytes < sizeof (pool_list_t)) {
        nbytes = sizeof (pool_list_t);
    }

    /*
     *  copy parameters
    */

    poolid -> flags = flags;
    poolid -> nmax = nels;
    poolid -> nbytes = nbytes;
    if (name && *name) {
        poolid -> name = strdup (name);
    }

    /*
     *  create mutex
    */

    if (pthread_mutex_init (&poolid -> mutex, NULL) != EOK) {
        free (poolid);
D       printf ("%s %d:  mpool_create couldn't pthread_mutex_init, errno %d (%s)\n", __FILE__, __LINE__, errno, strerror (errno));
        return (NULL);
    }

    /*
     *  see what kind of pool we're creating.
    */

    switch (flags & MPOOL_TYPE_MASK) {
    case    MPOOL_TYPE_STATIC:
        if (nels == 0) {                                                        // infinite sized static pools are illegal
            pthread_mutex_destroy (&poolid -> mutex);
            free (poolid);
D           printf ("%s %d:  mpool_create couldn't create static pool of infinite size!\n", __FILE__, __LINE__);
            return (NULL);
        }

        /*
         *  Allocate the entries in the static pool.  For
         *  maximum efficiency, we allocate one big chunk and
         *  then manage it ourselves in a linked list.  Fill
         *  in the "big chunk" in the ".root" member (so we can
         *  free it later when/if we destroy this pool).
        */

        poolid -> root = malloc (nbytes * nels);
        if (poolid -> root == NULL) {
D           printf ("%s %d:  couldn't allocate static root pool entry (%d x %d = %d bytes)\n", __FILE__, __LINE__, nbytes, nels, nbytes * nels);
            pthread_mutex_destroy (&poolid -> mutex);
            free (poolid);
            return (NULL);
        }
D       printf ("%s %d:  allocated static pool entry (%d x %d = %d bytes) [%p -> %p address range]\n", __FILE__, __LINE__, nbytes, nels, nbytes * nels, poolid -> root, ((char *) poolid -> root) + (nbytes * nels - 1));

        // now resize the pool into "nbytes"-sized chunks (we'll do "nels" of them)

        charptr = poolid -> root;                                               // assign to "charptr" so we can do math
        for (i = 0; i < nels; i++) {
            ptrlist = (pool_list_t *) charptr;                                  // base list gets the current "next free" entry from "allocator"
            charptr += nbytes;                                                  // advance allocator to next chunk
            poolid -> nalloc++;                                                 // just allocated one, indicate this
            poolid -> nfree++;                                                  // we also have one on the free list, 'cuz we're creating the free list
            ptrlist -> next = poolid -> list;                                   // assign the next to be the current head
            poolid -> list = ptrlist;                                           // assign the head to point to us
        }
        break;
    }

D   printf ("%s %d:  mpool_create -> %p\n", __FILE__, __LINE__, poolid);
    return (poolid);
}

int
mpool_destroy (mpool_t *poolid)
{
    void        *next;

    pthread_mutex_lock (&poolid -> mutex);

        /*
         *  we must free all entries on the pool before we destroy the pool
        */

        switch (poolid -> flags & MPOOL_TYPE_MASK) {
        case    MPOOL_TYPE_STATIC:
            if (poolid -> nalloc != poolid -> nmax) {
D               printf ("%s %d:  attempt to remove a static pool while some elements are in use.  BAD BAD BAD!\n", __FILE__, __LINE__);
            }
            free (poolid -> root);
            break;

        default:
            while (poolid -> list) {
                next = poolid -> list -> next;
                free (poolid -> list);
                poolid -> list = next;
            }
            break;
        }

        /*
         *  unlock and destroy the mutex -- yes, this is a tiny window
         *  but only if someone calls mpool_destroy() while the pool is
         *  in use -- technically, this is "illegal" :-)
        */

    pthread_mutex_unlock (&poolid -> mutex);
    pthread_mutex_destroy (&poolid -> mutex);

    if (poolid -> name) {
        free (poolid -> name);
    }

    /*
     *  now destroy the poolid object
    */

    free (poolid);

    return (EOK);
}

/*
 *  mpool_malloc
 *  mpool_calloc
 *  mpool_free
*/

static void *
mpool_malloc_wml (mpool_t *poolid)
{
    void    *ptr;

    /*
     *  If there's something in the pool, we return it.
     *  Note that this is done regardless of the policy and
     *  constraints -- the thinking is that if we have something
     *  in the pool, it must have been returned legitimately.
    */

    if (poolid -> list) {
        poolid -> nfree--;                                                      // decrement free count 'cuz we just took one
        ptr = poolid -> list;
        poolid -> list = poolid -> list -> next;
        return (ptr);
    }

    /*
     *  see if there's room to allocate...
    */

    if (poolid -> nmax) {                                                       // if there are limits, check them
        if (poolid -> nalloc >= poolid -> nmax) {
            return (NULL);
        }
    }

    /*
     *  At this point, we have permission to allocate -- allocate
     *  according to the policy (common code above may not apply
     *  to every policy).
    */

    switch (poolid -> flags & MPOOL_TYPE_MASK) {
    case    MPOOL_TYPE_DYNAMIC:
    case    MPOOL_TYPE_NEVER_SHRINK:
        ptr = malloc (poolid -> nbytes);
        if (ptr) {
            poolid -> nalloc++;
        }
        return (ptr);

    case    MPOOL_TYPE_STATIC:
        return (NULL);                                                          // if we're here, then nothing in the pool, which means we've failed.
    }

    return (NULL);
}

void *
mpool_malloc (mpool_t *poolid)
{
    void    *ptr;

    pthread_mutex_lock (&poolid -> mutex);

        ptr = mpool_malloc_wml (poolid);                                        // WML == With Mutex Locked -- there are too many returns() in the worker to remember to unlock the mutex, so we wrap it

    pthread_mutex_unlock (&poolid -> mutex);

D   printf ("%s %d:  mpool_malloc poolid %p returns %p\n", __FILE__, __LINE__, poolid, ptr);
    return (ptr);
}

void *
mpool_calloc (mpool_t *poolid)
{
    void    *ptr;

    if ((ptr = mpool_malloc (poolid)) != NULL) {
        memset (ptr, 0, poolid -> nbytes);
    }
    return (ptr);
}

int
mpool_free (mpool_t *poolid, void *ptr)
{
    pool_list_t     *pptr;

D   printf ("%s %d:  mpool_free poolid %p pointer %p\n", __FILE__, __LINE__, poolid, ptr);
    pthread_mutex_lock (&poolid -> mutex);

        poolid -> nalloc--;
        poolid -> nfree++;

        switch (poolid -> flags & MPOOL_TYPE_MASK) {
        case    MPOOL_TYPE_DYNAMIC:
            free (ptr);
            break;

        case    MPOOL_TYPE_NEVER_SHRINK:
        case    MPOOL_TYPE_STATIC:
            pptr = ptr;
            pptr -> next = poolid -> list;
            poolid -> list = ptr;
            break;
        }

    pthread_mutex_unlock (&poolid -> mutex);

    return (EOK);
}

void
mpool_dump (mpool_t *poolid)
{
    printf ("Dump of pool %s\n", poolid -> name ? poolid -> name : "");
    printf ("\telement size:                %d bytes\n", poolid -> nbytes);
    printf ("\tmaximum number of elements:  ");
        if (poolid -> nmax) {
            printf ("%d\n", poolid -> nmax);
        } else {
            printf ("(infinite)\n");
        }
    printf ("\telements allocated:          %d\n", poolid -> nalloc);
    printf ("\tpool type:                 0x%08X ", poolid -> flags);
        switch (poolid -> flags & MPOOL_TYPE_MASK) {
        case    MPOOL_TYPE_DYNAMIC:         printf ("MPOOL_TYPE_DYNAMIC ");         break;
        case    MPOOL_TYPE_NEVER_SHRINK:    printf ("MPOOL_TYPE_NEVER_SHRINK ");    break;
        case    MPOOL_TYPE_STATIC:          printf ("MPOOL_TYPE_STATIC ");          break;
        }
    printf ("\n");
}

void
mpool_info (mpool_t *poolid, size_t *nbytes, uint32_t *nmax, uint32_t *nalloc, uint32_t *nfree, uint32_t *flags, char **name)
{
    if (nbytes) *nbytes = poolid -> nbytes;
    if (nmax) *nmax = poolid -> nmax;
    if (nalloc) *nalloc = poolid -> nalloc;
    if (nfree) *nfree = poolid -> nfree;
    if (flags) *flags = poolid -> flags;
    if (name) *name = poolid -> name;
}

