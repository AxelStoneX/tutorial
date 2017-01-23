
/*
 *  c_open.c
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
 *  This module is the RAMDISK's c_open handler.
 *
 *  CURRENTLY NO EFFORT IS MADE TO BE THREAD SAFE
 *
 *  2002 08 13  R. Krten        created
 *  2002 10 29  R. Krten        separated
 *  2002 12 05  R. Krten        thought long and hard about c_open() logic
 *  2002 12 21  R. Krten        change io_ to c_ for "connect"
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

static void redirect_symlink (resmgr_context_t *ctp, struct _io_connect *cmsg, cfs_attr_t *attr, des_t *components, int ncomponents);

/*
 *  cfs_c_open()
 *
 *  This function is the default CFS 'open' handler function.
 *
 *  At the highest level, it's responsible for opening the pathname specified
 *  by the connect message and binding an OCB.
 *
 *  As simple as that sounds, there's actually quite a bit involved:
 *
 *  1) find the target:
 *      a) this is accomplished by calling connect_msg_to_attr().  If that
 *         function detects any errors or needs to perform symlink redirection,
 *         it does so, and returns a non-zero, indicating that we should
 *         pass its status and terminate.
 *
 *  2) analyze the mode flags:
 *      a) if both O_CREAT and O_EXCL set, target must not exist (else EEXIST)
 *      b) if O_CREAT set, target may or may not exist
 *      c) if O_CREAT not set, target must exist (else ENOENT)
 *      d) if O_TRUNC and either O_RDWR or O_WRONLY, target set to 0 length
 *      e... etc
 *
 *  3) create target if required (i.e., no target and O_CREAT)
 *      a) call internal cfs_a_mkfile() to create our data structures
 *
 *  4) truncate target if required (i.e., O_TRUNC and write mode)
 *
 *  5) at this point, there is a target.  Call the iofunc_() and
 *     resmgr_() functions to bind to OCB.
*/

int
cfs_c_open (resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra)
{
    int         sts;
    des_t       parent, target;
    struct  _client_info cinfo;

    if (sts = iofunc_client_info (ctp, 0, &cinfo)) {
        return (sts);
    }

    if (connect_msg_to_attr (ctp, &msg -> connect, handle, &parent, &target, &sts, &cinfo)) {
        return (sts);
    }

D   printf ("%s %d:  cfs_c_open, after magic, sts is %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
    if (!target.attr) {
D       printf ("%s %d:  cfs_c_open, target doesn't exist\n", __FILE__, __LINE__);
        if (!(msg -> connect.ioflag & O_CREAT)) {
D           printf ("%s %d:  not creating and doesn't exist, ENOENT\n", __FILE__, __LINE__);
            return (ENOENT);
        }

        sts = iofunc_open (ctp, msg, NULL, &parent.attr -> attr, NULL);
        if (sts != EOK) {
            cfs_a_rmattr (parent.attr, target.attr);
D           printf ("%s %d: cfs_c_open iofunc_open returned %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
            return (sts);
        }

        target.attr = cfs_a_mkfile (parent.attr, target.name, &cinfo);  // the "unextended" part of the attribute gets zapped later by iofunc_open()
        if (!target.attr) {
D           printf ("%s %d: cfs_c_open couldn't cfs_a_mkfile target %s errno %d (%s)\n", __FILE__, __LINE__, target.name, errno, strerror (errno));
            return (errno);
        }

    } else {

D       printf ("%s %d:  cfs_c_open, target exists\n", __FILE__, __LINE__);
        sts = iofunc_open (ctp, msg, &target.attr -> attr, NULL, NULL);
        if (sts != EOK) {
D           printf ("%s %d: cfs_c_open iofunc_open returned %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
            return (sts);
        }
    }

    //  Target existed or has been created.  Truncate if required
    if (msg -> connect.ioflag & O_TRUNC) {
        cfs_a_truncate (target.attr, 0, TRUNCATE_ERASE);                        // truncate at offset zero 'cuz we're opening it
    }
    
    sts = iofunc_ocb_attach (ctp, msg, NULL, &target.attr -> attr, NULL);
    if (sts != EOK) {
D       printf ("%s %d: cfs_c_open iofunc_ocb_attach returned %d (%s)\n", __FILE__, __LINE__, sts, strerror (sts));
    }

    return (sts);
}

/*
 *  redirect_symlink
 *
 *  This function fills the reply buffer with the symlink redirection.
 *
 *  Thanks to dagibbs@qnx.com for providing the skeleton code for this
 *  function, and to dagibbs@qnx.com and xtang@qnx.com for clarifying
 *  the functionality required.
 *
 *  To handle a symbolic link redirect, we need to know if the target
 *  of the symlink is relative or absolute.  The following table
 *  applies:
 *
 *  relative:  cwd + content + rest
 *  absolute:  content + rest
 *
 *  Where:
 *      cwd
 *          the current working directory UP TO BUT NOT
 *          INCLUDING the symlink
 *      content
 *          the content of the symlink
 *      rest
 *          the rest of the pathname after the symlink (if any)
 *
 *  Furthermore, things get a little messy, because we no
 *  longer have a clean "cwd" nor "rest" -- remember, we've broken them
 *  up with "pathwalk()".  So we need to do some reconstructive surgery.
 *  Let's face it, we're pigs -- we just allocate a string that's "big
 *  enough" and reconstruct the path into that.
*/

static void
redirect_symlink (resmgr_context_t *ctp, struct _io_connect *msg, cfs_attr_t *attr, des_t *components, int ncomponents)
{
    int     eflag;
    int     ftype;
    char    newpath [PATH_MAX];
    int     i;
    char    *p;
    struct _io_connect_link_reply       link_reply;

    /*
     *  Reconstruct the original pathname by copying pathname components
     *  into "newpath".
    */

D   printf ("%s %d:  redirect_symlink (attr %p ncomponents %d)\n", __FILE__, __LINE__, attr, ncomponents);

    i = 1;
    p = newpath;                                                                // optimize our strcat search time
    *p = 0;

    if (*attr -> type.symlinkdata != '/') {                                     // a relative path, do up to the symlink itself
D       printf ("%s %d:  redirect_symlink detects a relative path\n", __FILE__, __LINE__);
        for (; i < (ncomponents - 1); i++) {
D           printf ("%s %d:  redirect_symlink adding component %d [%s]\n", __FILE__, __LINE__, i, components [i].name);
            strcat (p, components [i].name);
            p += strlen (p);
            strcat (p, "/");
            p++;
D           printf ("%s %d:  redirect_symlink has now built [%s]\n", __FILE__, __LINE__, newpath);
        }
    } else {
        i = ncomponents - 1;
    }

    // now substitute the content of the symlink
    strcat (p, attr -> type.symlinkdata);
    p += strlen (p);
D   printf ("%s %d:  redirect_symlink has now substituted the content of the symlink (%s) to get [%s]\n", __FILE__, __LINE__, attr -> type.symlinkdata, newpath);

    i++;                                                                        // skip the symlink itself now that we've substituted it

    for (; components [i].name && i < PATH_MAX; i++) {
D       printf ("%s %d:  redirect_symlink adding component %d [%s]\n", __FILE__, __LINE__, i, components [i].name);
        strcat (p, "/");
        strcat (p, components [i].name);
        p += strlen (p);
D       printf ("%s %d:  redirect_symlink has now built [%s]\n", __FILE__, __LINE__, newpath);
    }

    // preserve these...
    eflag = msg -> eflag;
    ftype = msg -> file_type;

    memset (&link_reply, 0, sizeof (link_reply));

    _IO_SET_CONNECT_RET (ctp, _IO_CONNECT_RET_LINK);
    link_reply.file_type = ftype;
    link_reply.eflag = eflag;
    link_reply.path_len = strlen (newpath) + 1;
    SETIOV (&ctp -> iov [0], &link_reply, sizeof (link_reply));
    SETIOV (&ctp -> iov [1], newpath, link_reply.path_len);

    MsgReplyv (ctp -> rcvid, ctp -> status, ctp -> iov, 2);
}

/*
 *  connect_msg_to_attr
 *
 *  This function is responsible for performing the first level of work
 *  when resolving names and converting them to attributes structures:
 *
 *      a) this is accomplished by calling pathwalk() to split the pathname
 *         into individual components, and verify that each component exists,
 *         is accessible (i.e., the client has the appropriate privileges),
 *         and is of the correct type (it has to be a directory if there are
 *         to be other files/directories within it).
 *
 *      b) several scenarios are possible:
 *          i)   all components within the pathname were accessible, of
 *               the correct type, and present.  In this case, pathname
 *               processing is done, and we can continue on to the next step (a
 *               zero value is returned).
 *          ii)  as above, except that the final component doesn't exist.  In
 *               this case, we *may* be done; it depends on if we're creating
 *               the final component or not (a zero value is returned, but
 *               "rval" is set to ENOENT).  We leave it to a higher level to
 *               determine if the final component was required.
 *          iii) a component in the pathname was not a directory, does not
 *               exist, or the client doesn't have permission to access it.
 *               In this case, we're done as well, but we abort with an errno
 *               return (a non-zero is returned, "rval" is set to the errno).
 *          iv)  a component in the pathname is a symbolic link.  In this case,
 *               we're done as well, and we perform a symlink redirect.  A
 *               non-zero is returned, which should be passed up to the
 *               resource manager framework of the caller.
 *
 *  This function accepts:
 *
 *      ctp, cmsg, handle
 *          - these are the standard resource manager data items (cmsg is the ".i" member of all connect messages)
 *      parent, target
 *          - these are pointers to the parent and target structures that are
 *            filled as a result of calling this routine
 *      sts
 *          - this is the status
 *
 *  This function returns:
 *      0
 *          - no errors or other "early out" conditions were detected.
 *      non zero
 *          - an error or early out condition was detected; upper level should just
 *            pass along value of "sts" and abort.
*/

int
connect_msg_to_attr (resmgr_context_t *ctp, struct _io_connect *cmsg, RESMGR_HANDLE_T *handle, des_t *parent, des_t *target, int *sts, struct _client_info *cinfo)
{
    des_t       components [_POSIX_PATH_MAX];
    int         ncomponents;

D   printf ("%s %d:  connect_msg_to_attr (%s) mode 0X%08X eflag 0X%08X IOFLAG:", __FILE__, __LINE__, cmsg -> path, cmsg -> mode, cmsg -> eflag); dump_ioflag (cmsg -> ioflag);

    //  Find the target, and validate accessability of components

    ncomponents = _POSIX_PATH_MAX;
    *sts = pathwalk (ctp, cmsg -> path, handle, 0, components, &ncomponents, cinfo);
D   dump_pw (*sts, ncomponents, components);

    *target = components [ncomponents - 1];
    *parent = ncomponents == 1 ? *target : components [ncomponents - 2];

    // missing directory component, or can't access
    if (*sts == ENOTDIR || *sts == EACCES) {
D       printf ("%s %d:  connect_msg_to_attr returns %d (%s)\n", __FILE__, __LINE__, *sts, strerror (*sts));
        return (1);
    }

    // missing non-final component
    if (components [ncomponents].name != NULL && *sts == ENOENT) {
D       printf ("%s %d:  connect_msg_to_attr missing non-final component, returns %d (%s)\n", __FILE__, __LINE__, *sts, strerror (*sts));
        return (1);
    }

    // check final component

    if (*sts == EOK) {
        // if they wanted a directory, and we aren't one, honk.
        if (S_ISDIR (cmsg -> mode) && !S_ISDIR (components [ncomponents - 1].attr -> attr.mode)) {
            *sts = ENOTDIR;
            return (1);
        }
        if (S_ISLNK (components [ncomponents - 1].attr -> attr.mode) && (components [ncomponents].name || (cmsg -> eflag & _IO_CONNECT_EFLAG_DIR) || !S_ISLNK (cmsg -> mode))) {
D           printf ("%s %d:  connect_msg_to_attr redirecting symlink\n", __FILE__, __LINE__);
            redirect_symlink (ctp, cmsg, target -> attr, components, ncomponents);
            *sts = _RESMGR_NOREPLY;
            return (1);
        }
    }
    return (0);
}

