
/*
 *  pathname.c
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
 *  This module is the RAMDISK's pathname handling
 *  functions.
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

static int add_blocks (cfs_attr_t *attr, int sblk, int nblks, int flags);
static void wipe_block_to_end (cfs_attr_t *attr, int blk, int offset);
static void remove_blocks (cfs_attr_t *attr, int sblk, int nblks, int flags);

/*
 *  cfs_a_mkfile
 *  cfs_a_mkdir
 *
 *  Entry point to create a file or directory based on an attribute.  The attribute that's
 *  passed is the "current directory" in which the entity is created.  The
 *  null parameter defines the characteristics of the entity (currently not used).
*/

cfs_attr_t *
cfs_a_mknod (cfs_attr_t *attr, char *fname, int mode, struct _client_info *cinfo)
{
    cfs_attr_t  *newattr;

D   if (cinfo) printf ("%s %d:  cfs_a_mknod (attr %p, fname \"%s\", mode 0x%x (0%o), cinfo [nd %d pid %d  sid %d flags %d cred -> [ruid %d euid %d suid %d rgid %d egid %d sgid %d ngroups %d]]\n", __FILE__, __LINE__,
            attr, fname, mode, mode, cinfo -> nd, cinfo -> pid, cinfo -> sid, cinfo -> flags,
            cinfo -> cred.ruid, cinfo -> cred.euid, cinfo -> cred.suid, cinfo -> cred.rgid, cinfo -> cred.egid, cinfo -> cred.sgid, cinfo -> cred.ngroups);
D   if (!cinfo) printf ("%s %d:  cfs_a_mknod (attr %p, fname \"%s\", mode 0x%x (0%o), cinfo NULL\n", __FILE__, __LINE__,  attr, fname, mode, mode);
        
    newattr = add_new_node_and_dirent (attr, fname);
    if (!newattr) {
        return (NULL);
    }

    cfs_attr_init (newattr);
    iofunc_attr_init (&newattr -> attr, mode, &attr -> attr, 0);
    newattr -> attr.inode = (int) newattr;
    newattr -> attr.atime = newattr -> attr.mtime = newattr -> attr.ctime = time (NULL);    // they are all created equal...
    newattr -> attr.rdev = 7;
    if (cinfo) {
        newattr -> attr.uid = cinfo -> cred.euid;
        newattr -> attr.gid = cinfo -> cred.egid;
    } // else defaults to root:root

    return (newattr);
}

cfs_attr_t *
cfs_a_mkfile (cfs_attr_t *attr, char *fname, struct _client_info *cinfo)
{
    return (cfs_a_mknod (attr, fname, S_IFREG | 0644, cinfo));
}

cfs_attr_t *
cfs_a_mkdir (cfs_attr_t *attr, char *fname, struct _client_info *cinfo)
{
    cfs_attr_t  *new;

    new = cfs_a_mknod (attr, fname, S_IFDIR | 0755, cinfo);
    if (new) {
        cfs_a_mknod (new, ".", S_IFDIR | 0755, cinfo);
        cfs_a_mknod (new, "..", S_IFDIR | 0755, NULL);                          // ".." should *not* get anything from this client...
    }
    return (new);
}

cfs_attr_t *
cfs_a_mksymlink (cfs_attr_t *attr, char *fname, void *null)
{
    return (cfs_a_mknod (attr, fname, S_IFLNK | 0777, NULL));
}

/*
 *  cfs_a_truncate (attr, offset, flags)
 *
 *  Shrinks or grows the file specified by "attr" to the new size "offset".
 *  Flags are selected from the following:
 *
 *      TRUNCATE_ERASE              the new data blocks are zeroed
 *      TRUNCATE_ALL_OR_NONE        either all of the blocks are allocated,
 *                                  or none are -- this is used for disk-full
 *                                  situations (write() wants this flag off,
 *                                  truncate() wants the flag on).
*/

int
cfs_a_truncate (cfs_attr_t *attr, off_t offset, int flags)
{
    int     nblks;
    int     sblk, eblk;
    int     ntoadd, nadded;
    int     soff;

D   printf ("%s %d:  cfs_a_truncate attr %p (nbytes %lld) newsize %d flags 0x%08X\n", __FILE__, __LINE__, attr, attr -> attr.nbytes, offset, flags);

    if (offset == attr -> attr.nbytes) {                                        // same size as current; ignore
D       printf ("%s %d:  cfs_a_truncate attr %p NOOP sizes equal, early out\n", __FILE__, __LINE__, attr);
        return (EOK);
    }

    /*
     *  At this point, we could be growing or shrinking.
     *  Calculate the start and end blocks, as well as the start offset.
    */

    if (offset > attr -> attr.nbytes) {                                         // growing

        /*
         *  sblk = the block number after the last valid byte; so if size was 512, we want block 1, not block 0
         *  eblk = the block number that contains the last new byte; so if new size was 1024, we want block 1, not block 2
         *  soff = the first byte that's new, so if the new size is 1 byte, the first new byte is byte 1, not 0.
        */

        sblk = attr -> attr.nbytes / BLOCKSIZE;
        eblk = (offset - 1) / BLOCKSIZE;
        soff = attr -> attr.nbytes & (BLOCKSIZE - 1);
D       printf ("%s %d:  cfs_a_truncate growing %lld to %d, sblk %d soff %d eblk %d\n", __FILE__, __LINE__, attr -> attr.nbytes, offset, sblk, soff, eblk);

        /*
         *  if we're growing, we:
         *      allocate (eblk - nblocks + 1) blocks
         *      wipe (sblk, soff, eblk)
        */

        nblks = (attr -> attr.nbytes + BLOCKSIZE - 1) / BLOCKSIZE;              // really should be same as "nels" though...
        ntoadd = eblk - nblks + 1;

        /*
         *  allocate the blocks (if required -- we could just be growing the
         *  file by 1 byte, with no additional blocks required)
        */

        if (ntoadd) {
            nadded = add_blocks (attr, nblks, ntoadd, flags | TRUNCATE_ERASE);
            if (nadded != ntoadd) {                                             // if we didn't get all the blocks we wanted
                if (flags & TRUNCATE_ALL_OR_NONE) {                             // and we wanted all-or-none
                    remove_blocks (attr, nblks, nadded, 0);
D                   printf ("%s %d:  cfs_a_truncate didn't get all blocks and specified ALL_OR_NONE, killing %d blocks starting at %d, returning ENOSPC\n", __FILE__, __LINE__, nadded, nblks);
                    return (ENOSPC);
                }
                // else we wanted as-many-as-possible; recalculate our new "offset" (max size)
                offset = (nblks + nadded) * BLOCKSIZE;                          // this should *never* be bigger than "offset" (otherwise, what exactly was it the we *failed* to add?  Spare, bogus, space-alien blocks???)
            }
        }

        /*
         *  wipe the last few bytes in the *existing* block -- we used
         *  TRUNCATE_ERASE above to get clean ones, so there's no
         *  need to wipe those.  Note that "existing" block may in fact
         *  not exist :-) if we are just creating the file, so check for
         *  that case too.
        */

        if (sblk < attr -> nels) {
            wipe_block_to_end (attr, sblk, soff);
        }

    } else {                                                                    // shrinking

        /*
         *  sblk = the first block that does not contain a valid byte; so if the new size is 512, we want block 1, not 0.
         *  eblk = the last block that contained data, so if the old size was 513, we want block 1, not 0.
         *  soff = the first byte to be deleted, so if the new size is 1, the offset should be 1.
        */

        sblk = offset / BLOCKSIZE;
        eblk = (attr -> attr.nbytes - 1) / BLOCKSIZE;
        soff = offset & (BLOCKSIZE - 1);
D       printf ("%s %d:  cfs_a_truncate shrinking %lld to %d, sblk %d soff %d eblk %d\n", __FILE__, __LINE__, attr -> attr.nbytes, offset, sblk, soff, eblk);

        /* if we're shrinking, we:
         *      free (sblk + 1, eblk) [inclusive]
         *      wipe (sblk, soff, sblk) [wipe to end of block]
         *  Note that if the size is zero, we need special code to free block 0.
        */

        remove_blocks (attr, sblk + 1, eblk - sblk, 0 /* flags */);
        wipe_block_to_end (attr, sblk, soff);

        // this is the special case for trimming the file to 0 bytes
        if (offset == 0) {
            remove_blocks (attr, 0, 1, 0);
        }

    }

    // update the new size of the file
D   printf ("%s %d:  cfs_a_truncate () adjusting attr -> nbytes to %d\n", __FILE__, __LINE__, offset);
    attr -> attr.nbytes = offset;

    return (EOK);
}

// returns the number of blocks actually added
static int
add_blocks (cfs_attr_t *attr, int sblk, int nblks, int flags)
{
    int     i;
    int     lastblock;
    iov_t   *newblocks;
    int     nalloc;

    lastblock = sblk + nblks;

D   printf ("%s %d:  add_blocks (attr %p (nels %d, nbytes %lld), sblk %d, nblks %d, flags 0x%08X) calc'd new lastblock %d\n", __FILE__, __LINE__, attr, attr -> nels, attr -> attr.nbytes, sblk, nblks, flags, lastblock);

    // a few sanity checks first...
    if (lastblock < attr -> nels) {                                             // do we really need to add more blocks?
D       printf ("%s %d:  add_blocks, sanity failed \"lastblock (%d) < attr -> nels (%d)\" was true\n", __FILE__, __LINE__, lastblock, attr -> nels);
        return (0);                                                             // nope, so tell them we added 0 blocks
    }

    if (lastblock >= attr -> nalloc) {                                          // do we need more iov_t entries?
        newblocks = realloc (attr -> type.fileblocks, (lastblock + 1) * sizeof (iov_t));        // +1 'cuz it's a block number, not a count
        if (newblocks == NULL) {
D           printf ("%s %d:  add_blocks, couldn't realloc IOVs, old ptr %p, new size %d\n", __FILE__, __LINE__, attr -> type.fileblocks, (lastblock + 1) * sizeof (iov_t));
            return (0);
        }
        attr -> type.fileblocks = newblocks;                                    // assign

D       printf ("%s %d:  add_blocks now have %d blocks, nels is %d\n", __FILE__, __LINE__, lastblock + 1, attr -> nels);
        attr -> nalloc = lastblock + 1;                                         // since we extended the number allocated, we need to make a note of it; we'll fixup nels shortly...
    }

    // fill the new blocks from the block allocator
    nalloc = 0;
    for (i = attr -> nels; i < lastblock; i++) {
        attr -> type.fileblocks [i].iov_len = BLOCKSIZE;
        if ((attr -> type.fileblocks [i].iov_base = cfs_block_alloc (attr)) == NULL) {

            /*
             *  We ran out of room.  If the TRUNCATE_ALL_OR_NONE
             *  flag is set, we need to back out all of our allocations,
             *  and return (0).  Else, we simply bail out of the
             *  allocation loop.
            */

            if (flags & TRUNCATE_ALL_OR_NONE) {
                for (--i; i >= attr -> nels; i--) {                             // predecrement i 'cuz that one didn't work; no sense attempting to free a null one...
                    cfs_block_free (attr, attr -> type.fileblocks [i].iov_base);
                    attr -> type.fileblocks [i].iov_base = NULL;
                    attr -> type.fileblocks [i].iov_len = 0;
                }
D               printf ("%s %d:  add_blocks ALL_OR_NONE condition released blocks returning 0\n", __FILE__, __LINE__);
                return (0);
            }
D           printf ("%s %d:  add_blocks ran out of room at block index %d; not allocating rest\n", __FILE__, __LINE__, i);
            break;
        } else {
            nalloc++;
        }
D       printf ("%s %d:  add_blocks index [%d] got %p\n", __FILE__, __LINE__, i, attr -> type.fileblocks [i].iov_base);
    }
    attr -> nels += nalloc;                                                     // nels is adjusted here, because it has to reflect the "currently-used" size

D   printf ("%s %d:  add_blocks returning %d, nels %d\n", __FILE__, __LINE__, nalloc, attr -> nels);
    return (nalloc);
}

static void
wipe_block_to_end (cfs_attr_t *attr, int blk, int offset)
{
    char    *ptr;

D   printf ("%s %d:  wipe_block_to_end (attr %p (block %p), blk %d, offset %d)\n", __FILE__, __LINE__, attr, attr -> type.fileblocks [blk].iov_base, blk, offset);

    ptr = attr -> type.fileblocks [blk].iov_base;
    memset (ptr + offset, 0, BLOCKSIZE - offset);
}

static void
remove_blocks (cfs_attr_t *attr, int sblk, int nblks, int flags)
{
    int     i;

D   printf ("%s %d:  remove_blocks (attr %p, sblk %d, nblks %d, flags 0x%08X)\n", __FILE__, __LINE__, attr, sblk, nblks, flags);

    for (i = 0; i < nblks; i++) {
D       printf ("%s %d:  remove_blocks removing [%d] %p nels currently %d before removal\n", __FILE__, __LINE__, i + sblk, attr -> type.fileblocks [i + sblk].iov_base, attr -> nels);
        cfs_block_free (attr, attr -> type.fileblocks [i + sblk].iov_base);
        attr -> type.fileblocks [i + sblk].iov_base = NULL;
        attr -> nels--;
        // this might be a good place to realloc the iov_t via nalloc -- currently it will just grow to "max filesize used" and never shrink.
    }
}

void
cfs_a_rmattr (cfs_attr_t *parent, cfs_attr_t *target)
{
D   printf ("%s %d cfs_a_rmattr parent %p target %p *** UNIMPLEMENTED ***\n", __FILE__, __LINE__, parent, target);
}

static cfs_attr_t *
search_dir (char *component, cfs_attr_t *parent)
{
    int     i;

    for (i = 0; i < parent -> nels; i++) {
//D     printf ("%s %d:  search_dir (\"%s\" in parent %p) element [%d] (%s)\n", __FILE__, __LINE__, component, parent, i, parent -> type.dirblocks [i].name);

        if (parent -> type.dirblocks [i].name == NULL) {                        // skip empty ones
            continue;
        }
        if (!strcmp (parent -> type.dirblocks [i].name, component)) {           // found it!
D           printf ("%s %d:  search_dir (\"%s\" in parent %p) element [%d] (%s) MATCHED (%p)\n", __FILE__, __LINE__, component, parent, i, parent -> type.dirblocks [i].name, parent -> type.dirblocks [i].attr);

            return (parent -> type.dirblocks [i].attr);
        }
    }

    return (NULL);
}

/*
 *  pathwalk
 *
 *  This function performs two steps:
 *      a) the slash-separated pathname is broken into individual pathname
 *         components, and the ".name" members of the output structure array
 *         are filled with the components.
 *      b) checking is performed, starting at the root, to ensure accessability
 *         to each component.  Processing aborts when a component is not
 *         found, is not a directory and needs to be, the client doesn't have
 *         permissions to access the directory, or the entry is a symlink.
 *
 *  Note that *nrets is passed in as the number of elements in the output
 *  array, and E2BIG is returned if there are more elements present.
 *
 *  The return values from pathwalk are as follows:
 *
 *  -   "output" is filled with the pathnames and attribute structures of
 *      each pathname component.  The first element (output [0]) corresponds
 *      to the mountpoint, and as such, has a ".name" member of the empty
 *      string ("").
 *      Elements past the "*nrets" return value may have valid pathnames,
 *      as this is done in the first step to make it simple to detect if
 *      components follow a particular component.
 *  -   "nrets" contains the number of elements successfully processed.
 *      It will always be at least 1, because there is always at least
 *      the (implied) mountpoint.
*/

int
pathwalk (resmgr_context_t *ctp, char *pathname, cfs_attr_t *mountpoint, int flags, des_t *output, int *nrets, struct _client_info *cinfo)
{
    int                     nels;
    int                     sts;
    char                    *p;

D   printf ("%s %d:  pathwalk (\"%s\") at mountpoint %p with flags 0x%08x\n", __FILE__, __LINE__, pathname, mountpoint, flags);
D   printf ("%s %d:  pathwalk breaking %s into ", __FILE__, __LINE__, pathname); D fflush (stdout);

    // first, we break apart the slash-separated pathname
    memset (output, 0, sizeof (output [0]) * *nrets);
    output [0].attr = mountpoint;                                               // a given; we always start at the mountpoint
    output [0].name = "";                                                       // and the mountpoint is always the empty string

    for (nels = 1, p = strtok (pathname, "/"); p; p = strtok (NULL, "/"), nels++) {
        if (nels >= *nrets) {
            return (E2BIG);
        }
        output [nels].name = p;
D       printf ("%s ", p); D fflush (stdout);
        output [nels].attr = NULL;
    }
D   printf ("\n");

    // next, we analyze each pathname
    for (*nrets = 1; *nrets < nels; ++*nrets) {

D       printf ("%s %d:  pathwalk analyzing component \"%s\" (*nrets %d)\n", __FILE__, __LINE__, output [*nrets].name, *nrets);

        /*
         *  in order to be able to process another pathname component ("output [*nrets].name" in
         *  this case), the previous pathname component (*nrets - 1) must be
         *  a directory, and ...
        */

        if (!S_ISDIR (output [*nrets - 1].attr -> attr.mode)) {
D           printf ("%s %d:  pathwalk previous component %d (%s) -> ENOTDIR, therefore can't process this component (%s)\n", __FILE__, __LINE__, *nrets - 1, output [*nrets - 1].name, output [*nrets].name);
            return (ENOTDIR);       // *nrets includes the non-directory
        }

        /*
         *  ... we must have access permissions.
        */

        if ((sts = iofunc_check_access (ctp, &output [*nrets - 1].attr -> attr, S_IEXEC, cinfo)) != EOK) {
D           printf ("%s %d:  pathwalk component %d (%s) check_access returns %d (%s)\n", __FILE__, __LINE__, *nrets - 1, output [*nrets - 1].name, sts, strerror (sts));
            return (sts);           // *nrets includes the non-accessible directory
        }

        /*
         *  At this point we have the ability to do a directory seek, so we are
         *  actually working on behalf of the entry "output [*nrets].name"
         *  Check to see if the directory exists, if so...
        */

        if (!(output [*nrets].attr = search_dir (output [*nrets].name, output [*nrets - 1].attr))) {
D           printf ("%s %d:  pathwalk component %d (%s) within component %d (%s) -> ENOENT\n", __FILE__, __LINE__, *nrets, output [*nrets].name, *nrets - 1, output [*nrets - 1].name);
            ++*nrets;
            return (ENOENT);        // *nrets includes the entry
        }

D       printf ("%s %d:  pathwalk matched component %d (%s) -> %p\n", __FILE__, __LINE__, *nrets, output [*nrets].name, output [*nrets].attr);

        /*
         *  ... process the entry itself.
         *  We check to see if it's a symbolic link, and abort if it is.
         *  (Symbolic links are handled higher up).
        */

        if (S_ISLNK (output [*nrets].attr -> attr.mode)) {
D           printf ("%s %d:  pathwalk (\"%s\", component %d) -> S_ISLNK\n", __FILE__, __LINE__, pathname, *nrets);
            ++*nrets;
            return (EOK);           // *nrets includes the entry
        }
    }

    return (EOK);
}

/*
 *  dump_pw
 *
 *  A utility/debug function to dump the pathwalk output.
*/

void
dump_pw (int sts, int ncomponents, des_t *components)
{
    int     j;

    printf ("--pathwalk component dump: ncomponents %d, returns %d (%s)\n", ncomponents, sts, strerror (sts));
    for (j = 0; j < ncomponents; j++) {
        printf ("  [%d] attr %p name \"%s\"\n", j, components [j].attr, components [j].name);
    }
}
