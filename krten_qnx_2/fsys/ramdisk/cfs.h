
/*
 *  cfs.h
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
 *  This module contains the manifest constants and declarations for
 *  the RAMDISK..
 *
 *  2002 08 13  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

/*
 *  Required includes
*/

#include <dirent.h>
#include "../lib/mpool.h"

/*
 *  STRUCTURES
*/

struct cfs_attr_s;
struct des_s;

/*
 *  ATTRIBUTE OVERRIDES
*/

#define IOFUNC_ATTR_T   struct  cfs_attr_s

#include <sys/iofunc.h>

#include "ramdisk.h"

//  Extended Attributes Structures

typedef struct cfs_attr_s
{
    iofunc_attr_t       attr;                   // the regular one

    int                 nels;                   // number of elements (below) in use
    int                 nalloc;                 // number of elements (below) allocated (i.e., number of iov_t in fileblocks, or number of dirblocks, etc)
    union {
        struct des_s    *dirblocks;             // nels worth of directory blocks
        iov_t           *fileblocks;            // nels worth of file blocks
        char            *symlinkdata;           // just a plain, ordinary, strdup'd ASCII string.
    } type;
} cfs_attr_t;

//  Directory Entry Structure

typedef struct des_s
{
    char                *name;                  // name of entry
    cfs_attr_t          *attr;                  // attributes structure
}   des_t;

/*
 *  PROTOTYPES
*/

// externally visible
void                    cfs_func_init (resmgr_connect_funcs_t *cfuncs, resmgr_io_funcs_t *ifuncs);
void                    cfs_attr_init (cfs_attr_t *attr);
int                     cfs_rmnod (des_t *parent, char *name, cfs_attr_t *attr);
int                     release_attr (cfs_attr_t *attr);
cfs_attr_t              *cfs_a_mknod (cfs_attr_t *attr, char *fname, int mode, struct _client_info *cinfo);
cfs_attr_t              *cfs_a_mksymlink (cfs_attr_t *attr, char *fname, void *null);
cfs_attr_t              *cfs_a_mkdir (cfs_attr_t *attr, char *fname, struct _client_info *cinfo);
cfs_attr_t              *cfs_a_mkfile (cfs_attr_t *attr, char *fname, struct _client_info *cinfo);
void                    cfs_a_rmattr (cfs_attr_t *parent, cfs_attr_t *target);
#define TRUNCATE_ALL_OR_NONE        0x00000001
#define TRUNCATE_ERASE              0x00000002
int                     cfs_a_truncate (cfs_attr_t *attr, off_t offset, int flags /* TRUNCATE_* above */);
int                     add_new_dirent (cfs_attr_t *parent, cfs_attr_t *newattr, char *newname);
cfs_attr_t *            add_new_node_and_dirent (cfs_attr_t *attr, char *fname);
//int                       path_to_attr (resmgr_context_t *ctp, char *pathname, cfs_attr_t **pdattr, cfs_attr_t **pattr, int flags);
int                     pathwalk (resmgr_context_t *ctp, char *pathname, cfs_attr_t *mountpoint, int flags, des_t *output, int *nrets, struct _client_info *cinfo);
int                     connect_msg_to_attr (resmgr_context_t *ctp, struct _io_connect *cmsg, RESMGR_HANDLE_T *handle, des_t *parent, des_t *target, int *rval, struct _client_info *cinfo);
void                    dump_pw (int sts, int ncomponents, des_t *components);
void                    dump_c_connect (struct _io_connect *c);
int                     dirent_size (char *fname);
struct dirent           *dirent_fill (struct dirent *dp, int inode, int offset, char *fname);
void                    dump_ioflag (int ioflag);

// iofunc replacements (CONNECT)
int                     cfs_c_open        (resmgr_context_t *ctp, io_open_t *msg,     RESMGR_HANDLE_T *handle, void *extra);
int                     cfs_c_unlink      (resmgr_context_t *ctp, io_unlink_t *msg,   RESMGR_HANDLE_T *handle, void *reserved);
int                     cfs_c_rename      (resmgr_context_t *ctp, io_rename_t *msg,   RESMGR_HANDLE_T *handle, io_rename_extra_t *extra);
int                     cfs_c_mknod       (resmgr_context_t *ctp, io_mknod_t *msg,    RESMGR_HANDLE_T *handle, void *reserved);
int                     cfs_c_readlink    (resmgr_context_t *ctp, io_readlink_t *msg, RESMGR_HANDLE_T *handle, void *reserved);
int                     cfs_c_link        (resmgr_context_t *ctp, io_link_t *msg,     RESMGR_HANDLE_T *handle, io_link_extra_t *extra);
int                     cfs_c_unblock     (resmgr_context_t *ctp, io_pulse_t *msg,    RESMGR_HANDLE_T *handle, void *reserved);
int                     cfs_c_mount       (resmgr_context_t *ctp, io_mount_t *msg,    RESMGR_HANDLE_T *handle, io_mount_extra_t *extra);

// iofunc replacements (I/O)
int                     cfs_io_read       (resmgr_context_t *ctp, io_read_t *msg,     RESMGR_OCB_T *ocb);
int                     cfs_io_write      (resmgr_context_t *ctp, io_write_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_close_ocb  (resmgr_context_t *ctp, void *reserved,     RESMGR_OCB_T *ocb);
int                     cfs_io_stat       (resmgr_context_t *ctp, io_stat_t *msg,     RESMGR_OCB_T *ocb);
int                     cfs_io_notify     (resmgr_context_t *ctp, io_notify_t *msg,   RESMGR_OCB_T *ocb);
int                     cfs_io_devctl     (resmgr_context_t *ctp, io_devctl_t *msg,   RESMGR_OCB_T *ocb);
int                     cfs_io_unblock    (resmgr_context_t *ctp, io_pulse_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_pathconf   (resmgr_context_t *ctp, io_pathconf_t *msg, RESMGR_OCB_T *ocb);
int                     cfs_io_lseek      (resmgr_context_t *ctp, io_lseek_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_chmod      (resmgr_context_t *ctp, io_chmod_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_chown      (resmgr_context_t *ctp, io_chown_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_utime      (resmgr_context_t *ctp, io_utime_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_openfd     (resmgr_context_t *ctp, io_openfd_t *msg,   RESMGR_OCB_T *ocb);
int                     cfs_io_fdinfo     (resmgr_context_t *ctp, io_fdinfo_t *msg,   RESMGR_OCB_T *ocb);
int                     cfs_io_lock       (resmgr_context_t *ctp, io_lock_t *msg,     RESMGR_OCB_T *ocb);
int                     cfs_io_space      (resmgr_context_t *ctp, io_space_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_shutdown   (resmgr_context_t *ctp, io_shutdown_t *msg, RESMGR_OCB_T *ocb);
int                     cfs_io_mmap       (resmgr_context_t *ctp, io_mmap_t *msg,     RESMGR_OCB_T *ocb);
int                     cfs_io_msg        (resmgr_context_t *ctp, io_msg_t *msg,      RESMGR_OCB_T *ocb);
int                     cfs_io_dup        (resmgr_context_t *ctp, io_dup_t *msg,      RESMGR_OCB_T *ocb);
int                     cfs_io_close_dup  (resmgr_context_t *ctp, io_close_t *msg,    RESMGR_OCB_T *ocb);
int                     cfs_io_lock_ocb   (resmgr_context_t *ctp, void *reserved,     RESMGR_OCB_T *ocb);
int                     cfs_io_unlock_ocb (resmgr_context_t *ctp, void *reserved,     RESMGR_OCB_T *ocb);
int                     cfs_io_sync       (resmgr_context_t *ctp, io_sync_t *msg,     RESMGR_OCB_T *ocb);

/*
 *  DEBUG MACROS
*/

#define VDebug
#ifdef  VDebug
extern  int     optd;
#define CP  { if (optd) fprintf (stdout, "[** %s %d **]\n", __FILE__, __LINE__); }
#define D if (optd > 1)
#define malloc(s)  debug_malloc(__FILE__,__LINE__,s)
#define calloc(s,c)  debug_calloc(__FILE__,__LINE__,s,c)
#define realloc(p,s) debug_realloc(__FILE__,__LINE__,p,s)
#define strdup(s) debug_strdup(__FILE__,__LINE__,s)
#define free(p) debug_free(__FILE__,__LINE__,p)
void *debug_malloc (const char *f, int l, int s);
void *debug_calloc (const char *f, int l, int s, int c);
void *debug_realloc (const char *f, int l, void *p, int s);
char *debug_strdup (const char *f, int l, const char *s);
void debug_free (const char *f, int l, void *p);
#else   // VDebug
#define CP ;
#define D if (0)
#endif  // VDebug

// trailing includes

#include "../lib/block.h"

