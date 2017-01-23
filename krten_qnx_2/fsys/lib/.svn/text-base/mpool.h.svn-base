
/*
 *  mpool.h
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
 *  This module contains the manifest constants and declarations for
 *  the memory pool module.
 *
 *  2002 11 24  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>

/*
 *  Memory Pool Library
 *
 *  The memory pool library consists of functions to create/destroy
 *  memory pools, and to allocate/free chunks of memory within each
 *  individual pool.
 *
 *  The advantage of the memory pool is that it optionally enforces
 *  resource limits (a maximum number of chunks can be specified),
 *  and it also supports the concept of static pools, which are
 *  ideal for high-reliability systems.
 *
 *  To use the memory pool:
 *
 *      mpool_t     *pool_id;
 *
 *      pool_id = mpool_create (1234, 5678, MPOOL_TYPE_STATIC, "spud");
 *      if (pool_id == NULL) {
 *          fprintf (stderr, "Can't create memory pool\n");
 *          exit (EXIT_FAILURE);
 *      }
 *
 *      ptr = mpool_malloc (pool_id);
 *      ...
 *      mpool_free (pool_id, ptr);
 *
 *  The above example shows the creation of a pool that contains elements
 *  that are 1234 bytes in size, with a maximum number of 5678 such
 *  elements.  The flag MPOOL_TYPE_STATIC indicates that the elements
 *  are allocated "up front", meaning that the resource is guaranteed to
 *  be present, and when the elements are returned, they are returned to
 *  the pool, and not the operating system.  The pool is called "spud".
 *
 *  The mpool_malloc() function then fetches one of the elements, and the
 *  mpool_free() function returns it back to the pool.
 *
 *  The memory pool library is thread safe in the allocate/deallocate
 *  functions.  The create function does not need to be thread safe because
 *  there is no access to the mpool_t handle until after the function
 *  returns.  The destroy function does not need to be thread safe because
 *  it is illegal to call it concurrently with the allocate/deallocate
 *  functions (by definition).
*/

/*
 *  Pool Characteristics Flags
 *
 *  The flag constants here define the characteristics of the memory pool.
 *  MPOOL_TYPE_MASK defines the bit ranges used by the allocation policy,
 *  and can take on the following values:
 *
 *      MPOOL_TYPE_DYNAMIC
 *          In this mode, the library malloc() and free() functions are
 *          used to perform the allocation and deallocation of pool elements.
 *          The maximum number of elements is never exceeded; in the case
 *          of an infinite-sized upper limit, available memory is the true
 *          limit.
 *
 *      MPOOL_TYPE_NEVER_SHRINK
 *          In this mode, the pool starts off empty, and malloc() is used
 *          to bring in new pool elements, up to the maximum.  When a
 *          pool element is returned to the pool, it's placed in the pool
 *          rather than being returned to the operating system.
 *
 *      MPOOL_TYPE_STATIC
 *          In this mode, the pool is allocated up front, and all allocations
 *          and deallocations are done within the pool only.  This mode is
 *          most useful for critical systems where the pool memory must be
 *          guaranteed to be available.  In this mode, we cannot support the
 *          infinite pool size, as we'd have to allocate an infinite amount
 *          of memory up front.
 *
 *  Once a pool allocation policy is set (during mpool_create() time), it
 *  cannot be changed later.
*/

#define MPOOL_TYPE_MASK             0x00000003  // mask for pool "type" flags
#define MPOOL_TYPE_DYNAMIC          0x00000000  // pool grows and shrinks via malloc/free
#define MPOOL_TYPE_NEVER_SHRINK     0x00000001  // pool grows via malloc, never shrinks
#define MPOOL_TYPE_STATIC           0x00000002  // pool never grows nor shrinks
#define MPOOL_TYPE_RESERVED3        0x00000003  // reserved flag combination

/*
 *  pool_list_t
 *
 *  This data structure looks like it doesn't do anything -- in fact, it's
 *  what's used to hold the empty entries in a linked list.  When we free
 *  an entry, we put it on a list (the "list" member of mpool_t) that's
 *  composed of these pool_list_t datatypes.
*/

typedef struct pool_list_s
{
    struct pool_list_s  *next;
}   pool_list_t;

typedef struct mpool_s
{
    size_t          nbytes;                     // size of each element
    uint32_t        nmax;                       // maximum number of elements (0 == infinite)
    uint32_t        nalloc;                     // number currently allocated (total allocated from system)
    uint32_t        nfree;                      // number free (i.e., not returned to system)
    uint32_t        flags;                      // flags (from MPOOL_* above)
    char            *name;                      // optional name of the pool (useful for debug)
    pool_list_t     *list;                      // list of pool elements
    pthread_mutex_t mutex;                      // for mutex operations
    void            *root;                      // root of pool (for static pools only)
}   mpool_t;

// public prototypes

mpool_t *           mpool_create (size_t nbytes, uint32_t nels, uint32_t flags, const char *name);
int                 mpool_destroy (mpool_t *poolid);
void *              mpool_malloc (mpool_t *poolid);
void *              mpool_calloc (mpool_t *poolid);
int                 mpool_free (mpool_t *poolid, void *ptr);
void                mpool_dump (mpool_t *poolid);
void                mpool_info (mpool_t *poolid, size_t *nbytes, uint32_t *nmax, uint32_t *nalloc, uint32_t *nfree, uint32_t *flags, char **name);

