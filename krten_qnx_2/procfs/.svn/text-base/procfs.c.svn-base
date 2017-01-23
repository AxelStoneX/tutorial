
/*
 *  procfs.c
 *
 *  main.c shell version 0.005
 *
 *  Copyright 2003, 2012 QNX Software Systems Limited. All Rights Reserved.
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
 *  This module represents the main module for the /proc filesystem
 *  utility module.
 *
 *  This program will dump just about everything it can get its hands on from
 *  the /proc filesystem.  It serves as a cut-n-paste area where you can get
 *  various /proc information from.
 *
 *  2002 08 27  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
 *  2012 03 06  QSS             Minor corrections
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/procfs.h>
#include <sys/states.h>

static  void    optproc (int, char **);
static  void    dump_procfs_info (int fd, int pid);
static  void    dump_procfs_map_info (int fd, int pid);
static  void    dump_procfs_timer (int fd, int pid);
static  void    dump_procfs_irq (int fd, int pid);
static  void    dump_procfs_status (procfs_status *s);
static  void    dump_procfs_greg (int fd, int tid);
static  void    dump_procfs_fpreg (int fd);
static  void    dump_sigevent (struct sigevent *s);
static  void    dump_sig_info (siginfo_t *s);
static  void    iterate_process (int pid);
static  void    iterate_processes (void);
static  void    do_process (int pid, int fd, char *name);
static  void    do_thread (int fd, int tid, procfs_status *status);

const   char    *progname = "procfs";
const   char    *blankname= "      ";
extern  char    *version;                   // version.c

int     optv;

int
main (int argc, char **argv)
{
    optproc (argc, argv);

    iterate_processes ();

    return (EXIT_SUCCESS);
}

static void
usageError (void)
{
    fprintf (stderr, "%s:  error in use, type \"use %s\" for usage\n", progname, progname);
    exit (1);
}

static void
optproc (int argc, char **argv)
{
    int     opt;

    if (!argc) {
        usageError ();
    }

    while ((opt = getopt (argc, argv, "o:v")) != -1) {
        switch (opt) {
        case    'o':
            /* optarg has the string */
            break;
        case    'v':
            optv++;
            if (optv > 1) {
                printf ("Verbosity is %d\n", optv);
            }
            break;
        default:
            usageError ();
            break;
        }
    }

    for (; optind < argc; optind++) {
        printf ("additional parameters [%d] -%s-\n", optind, argv [optind]);
    }
}

static void
iterate_processes (void)
{
    struct dirent   *dirent;
    DIR             *dir;
    int             pid;

    // 1) find all processes
    if (!(dir = opendir ("/proc"))) {
        fprintf (stderr, "%s:  couldn't open /proc, errno %d\n", progname, errno);
        perror (NULL);
        exit (EXIT_FAILURE);
    }

    while (dirent = readdir (dir)) {
        // 2) we are only interested in process id's
        if (isdigit (*dirent -> d_name)) {
            pid = atoi (dirent -> d_name);
            iterate_process (pid);
        }
    }
    closedir (dir);
}

static void
iterate_process (int pid)
{
    char            paths [PATH_MAX];
    int             fd;

    // 1) set up structure
    static struct {
        procfs_debuginfo        info;
        char                    buff [PATH_MAX];
    } name;

    sprintf (paths, "/proc/%d/as", pid);

    if ((fd = open (paths, O_RDONLY)) == -1) {
        return;
    }

    // 2) ask for the name
    if (devctl (fd, DCMD_PROC_MAPDEBUG_BASE, &name, sizeof (name), 0) != EOK) {
        if (pid == 1) {
            strcpy (name.info.path, "(procnto)");
        } else {
            strcpy (name.info.path, "(n/a)");
        }
    }

    // 3) we can compare against name.info.path here...
    do_process (pid, fd, name.info.path);
    close (fd);
}

static void
do_process (int pid, int fd, char *name)
{
    procfs_status   status;

    printf ("PROCESS ID %d\n", pid);

    dump_procfs_info (fd, pid);
    dump_procfs_map_info (fd, pid);
    dump_procfs_timer (fd, pid);
    dump_procfs_irq (fd, pid);

    // now iterate through all of the threads
    status.tid = 1;
    while (1) {
        if (devctl (fd, DCMD_PROC_TIDSTATUS, &status, sizeof (status), 0) != EOK) {
            break;
        } else {
            do_thread (fd, status.tid, &status);
            status.tid++;
        }
    }
}

static void
dump_procfs_info (int fd, int pid)
{
    procfs_info info;
    int         sts;

    sts = devctl (fd, DCMD_PROC_INFO, &info, sizeof (info), NULL);
    if (sts != EOK) {
        fprintf (stderr, "%s:  error fetching DCMD_PROC_INFO for pid %d, errno %d (%s)\n", progname, pid, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    printf ("Info from DCMD_PROC_INFO\n");
    printf ("\tpid            %d\n", info.pid);
    printf ("\tparent         %d\n", info.parent);
    printf ("\tflags          0x%08X\n", info.flags);
    printf ("\tumask          0x%08X\n", info.umask);
    printf ("\tchild          %d\n", info.child);
    printf ("\tsibling        %d\n", info.sibling);
    printf ("\tpgrp           %d\n", info.pgrp);
    printf ("\tsid            %d\n", info.sid);
    printf ("\tbase_address   0x%016llX\n", info.base_address);
    printf ("\tinitial_stack  0x%016llX\n", info.initial_stack);
    printf ("\tuid            %d\n", info.uid);
    printf ("\tgid            %d\n", info.gid);
    printf ("\teuid           %d\n", info.euid);
    printf ("\tegid           %d\n", info.egid);
    printf ("\tsuid           %d\n", info.suid);
    printf ("\tsgid           %d\n", info.sgid);
    printf ("\tsig_ignore     0x%08lX-%08lX\n", info.sig_ignore.__bits [0], info.sig_ignore.__bits [1]);
    printf ("\tsig_queue      0x%08lX-%08lX\n", info.sig_queue.__bits [0], info.sig_queue.__bits [1]);
    printf ("\tsig_pending    0x%08lX-%08lX\n", info.sig_pending.__bits [0], info.sig_pending.__bits [1]);
    printf ("\tnum_chancons   %d\n", info.num_chancons);
    printf ("\tnum_fdcons     %d\n", info.num_fdcons);
    printf ("\tnum_threads    %d\n", info.num_threads);
    printf ("\tnum_timers     %d\n", info.num_timers);
    printf ("\tstart_time     0x%016llX\n", info.start_time);
    printf ("\tutime          %10lld.%09lld s\n", info.utime / 1000000000LL, info.utime % 1000000000LL);
    printf ("\tstime          %10lld.%09lld s\n", info.stime / 1000000000LL, info.stime % 1000000000LL);
    printf ("\tcutime         %10lld.%09lld s\n", info.cutime / 1000000000LL, info.cutime % 1000000000LL);
    printf ("\tcstime         %10lld.%09lld s\n", info.cstime / 1000000000LL, info.cstime % 1000000000LL);
    printf ("\tpriority       %d\n", info.priority);
    printf ("\n");
}

#define MAX_SEGMENTS        512

static void
dump_procfs_map_info (int fd, int pid)
{
    procfs_mapinfo  membufs [MAX_SEGMENTS];
    int             nmembuf;
    int             i;

    // fetch information about the memory regions for this pid
    if (devctl (fd, DCMD_PROC_PAGEDATA, membufs, sizeof (membufs), &nmembuf) != EOK) {
        fprintf (stderr, "%s:  couldn't get PAGEDATA information for process %d, errno %d (%s)\n", progname, pid, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    if (nmembuf > MAX_SEGMENTS) {
        fprintf (stderr, "%s:  process %d has more than %d memory segments (%d in fact) !!! ***\n", progname, pid, MAX_SEGMENTS, nmembuf);
        exit (EXIT_FAILURE);
    }

    printf ("Info from DCMD_PROC_PAGEDATA\n");
    printf ("\tBuff# ------vaddr------- -------size------- ---flags-- ---dev---- -------ino--------\n");
    for (i = 0; i < nmembuf; i++) {
        printf ("\t[%3d] ", i);
        printf ("0x%016llX ", membufs [i].vaddr);
        printf ("0x%016llX ", membufs [i].size);
        printf ("0x%08X ", membufs [i].flags);
        printf ("0x%08X ", membufs [i].dev);
        printf ("0x%016llX\n", membufs [i].ino);
    }
    printf ("\n");
}

#define MAX_TIMERS          512

static void
dump_procfs_timer (int fd, int pid)
{
    procfs_timer    timers [MAX_TIMERS];
    int             ntimers;
    int             i;

    // fetch information about the timers for this pid
    if (devctl (fd, DCMD_PROC_TIMERS, timers, sizeof (timers), &ntimers) != EOK) {
        fprintf (stderr, "%s:  couldn't get TIMERS information for process %d, errno %d (%s)\n", progname, pid, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    if (ntimers > MAX_TIMERS) {
        fprintf (stderr, "%s:  process %d has more than %d timers (%d in fact) !!! ***\n", progname, pid, MAX_TIMERS, ntimers);
        exit (EXIT_FAILURE);
    }

    printf ("Info from DCMD_PROC_TIMERS\n");
    for (i = 0; i < ntimers; i++) {
        printf ("\tBuffer %3d timer ID %d\n", i, timers [i].id);
        printf ("\t\titime %10lld.%09lld s, %9lld.%09lld interval s\n", timers [i].info.itime.nsec / 1000000000LL, timers [i].info.itime.nsec % 1000000000LL, timers [i].info.itime.interval_nsec / 1000000000LL, timers [i].info.itime.interval_nsec % 1000000000LL);
        printf ("\t\totime %10lld.%09lld s, %9lld.%09lld interval s\n", timers [i].info.otime.nsec / 1000000000LL, timers [i].info.otime.nsec % 1000000000LL, timers [i].info.otime.interval_nsec / 1000000000LL, timers [i].info.otime.interval_nsec % 1000000000LL);
        printf ("\t\tflags 0x%08X\n", timers [i].info.flags);
        printf ("\t\ttid   %d\n", timers [i].info.tid);
        printf ("\t\tnotify %d\n", timers [i].info.notify);
        printf ("\t\tclockid %d\n", timers [i].info.clockid);
        printf ("\t\toverruns %d\n", timers [i].info.overruns);
        dump_sigevent (&timers [i].info.event);
    }
    printf ("\n");
}

#define MAX_IRQS            512

static void
dump_procfs_irq (int fd, int pid)
{
    procfs_irq      irqs [MAX_IRQS];
    int             nirqs;
    int             i;

    // fetch information about the IRQs for this pid
    if (devctl (fd, DCMD_PROC_IRQS, irqs, sizeof (irqs), &nirqs) != EOK) {
        fprintf (stderr, "%s:  couldn't get IRQS information for process %d, errno %d (%s)\n", progname, pid, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    if (nirqs > MAX_IRQS) {
        fprintf (stderr, "%s:  process %d has more than %d IRQs (%d in fact) !!! ***\n", progname, pid, MAX_IRQS, nirqs);
        exit (EXIT_FAILURE);
    }

    printf ("Info from DCMD_PROC_IRQS\n");
    for (i = 0; i < nirqs; i++) {
        printf ("\tBuffer %3d\n", i);
        printf ("\t\tpid        %d\n", irqs [i].pid);
        printf ("\t\ttid        %d\n", irqs [i].tid);
        printf ("\t\thandler    0x%08X\n", (unsigned int) irqs [i].handler);
        printf ("\t\tarea       0x%08X\n", (unsigned int) irqs [i].area);
        printf ("\t\tflags      0x%08X\n", irqs [i].flags);
        printf ("\t\tlevel      %d\n", irqs [i].level);
        printf ("\t\tmask_count %d\n", irqs [i].mask_count);
        printf ("\t\tid         %d\n", irqs [i].id);
        printf ("\t\tvector     %d\n", irqs [i].vector);
        dump_sigevent (&irqs [i].event);
    }
    printf ("\n");
}

static void
dump_sigevent (struct sigevent *s)
{
    printf ("\t\tevent (sigev_notify type %d)\n", s -> sigev_notify);
    switch (s -> sigev_notify) {
    case    SIGEV_NONE:
        printf ("\t\t\tSIGEV_NONE\n");
        break;
    case    SIGEV_SIGNAL:
        printf ("\t\t\tSIGEV_SIGNAL (sigev_signo %d, sigev_value.sival_int %d)\n", s -> sigev_signo, s -> sigev_value.sival_int);
        break;
    case    SIGEV_SIGNAL_CODE:
        printf ("\t\t\tSIGEV_SIGNAL_CODE (sigev_signo %d, sigev_value.sival_int %d, sigev_code %d)\n", s -> sigev_signo, s -> sigev_value.sival_int, s -> sigev_code);
        break;
    case    SIGEV_SIGNAL_THREAD:
        printf ("\t\t\tSIGEV_SIGNAL_THREAD (sigev_signo %d, sigev_value.sival_int %d, sigev_code %d)\n", s -> sigev_signo, s -> sigev_value.sival_int, s -> sigev_code);
        break;
    case    SIGEV_PULSE:
        printf ("\t\t\tSIGEV_PULSE (sigev_coid 0x%08X, sigev_value.sival_int %d, sigev_priority %d, sigev_code %d)\n", s -> sigev_coid, s -> sigev_value.sival_int, s -> sigev_priority, s -> sigev_code);
        break;
    case    SIGEV_INTR:
        printf ("\t\t\tSIGEV_INTR\n");
        break;
    case    SIGEV_UNBLOCK:
        printf ("\t\t\tSIGEV_UNBLOCK\n");
        break;
    default:
        printf ("\t\t\t*** unknown sigev_notify type\n");
        break;
    }
}

static const char *states [] =
{
    "DEAD",         /* 0  0x00 */
    "RUNNING",      /* 1  0x01 */
    "READY",        /* 2  0x02 */
    "STOPPED",      /* 3  0x03 */
    "SEND  ",       /* 4  0x04 */
    "RECEIVE",      /* 5  0x05 */
    "REPLY",        /* 6  0x06 */
    "STACK",        /* 7  0x07 */
    "WAITTHREAD",   /* 8  0x08 */
    "WAITPAGE",     /* 9  0x09 */
    "SIGSUSPEND",   /* 10 0x0a */
    "SIGWAITINFO",  /* 11 0x0b */
    "NANOSLEEP",    /* 12 0x0c */
    "MUTEX",        /* 13 0x0d */
    "CONDVAR",      /* 14 0x0e */
    "JOIN",         /* 15 0x0f */
    "INTR",         /* 16 0x10 */
    "SEM",          /* 17 0x11 */
    "WAITCTX",      /* 18 0x12 */
    "NET_SEND",     /* 19 0x13 */
    "NET_REPLY"     /* 20 0x14 */
};

static void
do_thread (int fd, int tid, procfs_status *s)
{
    printf ("\tTHREAD ID %d\n", tid);

    dump_procfs_status (s);
    dump_procfs_greg (fd, tid);
    dump_procfs_fpreg (fd);
}

static void
dump_procfs_status (procfs_status *s)
{
    printf ("\tInfo from DCMD_PROC_TIDSTATUS (procfs_status is %d bytes)\n", sizeof (*s));

    printf ("\t\tpid            %d\n", s -> pid);
    printf ("\t\ttid            %d\n", s -> tid);
    printf ("\t\tflags          0x%08X\n", s -> flags);
    printf ("\t\twhy            0x%04X\n", s -> why);
    printf ("\t\twhat           0x%04X\n", s -> what);
    printf ("\t\tip             0x%016llX\n", s -> ip);
    printf ("\t\tsp             0x%016llX\n", s -> sp);
    printf ("\t\tstkbase        0x%016llX\n", s -> stkbase);
    printf ("\t\ttls            0x%016llX\n", s -> tls);
    printf ("\t\tstksize        0x%08X\n", s -> stksize);
    printf ("\t\ttid_flags      0x%08X\n", s -> tid_flags);
    printf ("\t\tpriority       %d\n", s -> priority);
    printf ("\t\treal_priority  %d\n", s -> real_priority);
    printf ("\t\tpolicy         %d\n", s -> policy);
    printf ("\t\tstate          %d ", s -> state);
        if (s -> state < (sizeof (states) / sizeof (char *))) {
            printf ("(%s)\n", states [s -> state]);
        } else {
            printf ("(invalid)\n");
        }
    printf ("\t\tsyscall        0x%04X\n", s -> syscall);
    printf ("\t\tlast_cpu       %d\n", s -> last_cpu);
    printf ("\t\ttimeout        0x%08X\n", s -> timeout);
    printf ("\t\tlast_chid      %d\n", s -> last_chid);
    printf ("\t\tsig_blocked    0x%08lX-%08lX\n", s -> sig_blocked.__bits [0], s -> sig_blocked.__bits [1]);
    printf ("\t\tsig_pending    0x%08lX-%08lX\n", s -> sig_pending.__bits [0], s -> sig_pending.__bits [1]);
    dump_sig_info (&s -> info);
    printf ("\t\tstart_time     0x%016llx\n", s -> start_time);
    printf ("\t\tsutime         %10lld.%09lld\n", s -> sutime / 1000000000LL, s -> sutime % 1000000000LL);

    switch (s -> state) {
    case    STATE_DEAD:
    case    STATE_RUNNING:
    case    STATE_READY:
        // nothing else to print
        break;
    case    STATE_STOPPED:
        break;
    case    STATE_SEND:
        printf ("\t\t\tSEND (nd %d pid %d coid 0x%08X chid 0x%08X scoid 0x%08X)\n", s -> blocked.connect.nd, s -> blocked.connect.pid, s -> blocked.connect.coid, s -> blocked.connect.chid, s -> blocked.connect.scoid);
        break;
    case    STATE_RECEIVE:
        printf ("\t\t\tRECEIVE on chid %d\n", s -> blocked.channel.chid);
        break;
    case    STATE_REPLY:
        printf ("\t\t\tREPLY (nd %d pid %d coid 0x%08X chid 0x%08X scoid 0x%08X)\n", s -> blocked.connect.nd, s -> blocked.connect.pid, s -> blocked.connect.coid, s -> blocked.connect.chid, s -> blocked.connect.scoid);
        break;
    case    STATE_STACK:
        printf ("\t\t\tSTACK on size %d\n", s -> blocked.stack.size);
        break;
    case    STATE_WAITTHREAD:
        break;
    case    STATE_WAITPAGE:
        printf ("\t\t\tWAITPAGE on pid %d vaddr 0x%016X, flags 0x%08x\n", s -> blocked.waitpage.pid, s -> blocked.waitpage.vaddr, s -> blocked.waitpage.flags);
        break;
    case    STATE_SIGSUSPEND:
        break;
    case    STATE_SIGWAITINFO:
        break;
    case    STATE_NANOSLEEP:
        break;
    case    STATE_MUTEX:
        printf ("\t\t\tMUTEX on id %d sync %d\n", s -> blocked.sync.id, s -> blocked.sync.sync);
        break;
    case    STATE_CONDVAR:
        printf ("\t\t\tCONDVAR on id %d sync %d\n", s -> blocked.sync.id, s -> blocked.sync.sync);
        break;
    case    STATE_JOIN:
        printf ("\t\t\tJOIN on tid %d\n", s -> blocked.join.tid);
        break;
    case    STATE_INTR:
        break;
    case    STATE_SEM:
        printf ("\t\t\tSEM on id %d sync %d\n", s -> blocked.sync.id, s -> blocked.sync.sync);
        break;
    case    STATE_WAITCTX:
        break;
    case    STATE_NET_SEND:
        break;
    case    STATE_NET_REPLY:
        break;
    }

    printf ("\n");
}

static void
dump_sig_info (siginfo_t *s)
{
    printf ("\t\tsig_info:\n");
    printf ("\t\t\tsi_signo         %d\n", s -> si_signo);
    printf ("\t\t\tsi_code          %d\n", s -> si_code);
    printf ("\t\t\tsi_errno         %d\n", s -> si_errno);
    // there are more to print out here...
}

static void
dump_procfs_greg (int fd, int tid)
{
    procfs_greg     g;
    int             sts;

    // set the current thread
    if ((sts = devctl (fd, DCMD_PROC_CURTHREAD, &tid, sizeof (tid), NULL)) != EOK) {
        fprintf (stderr, "%s:  couldn't set CURTHREAD information for tid %d, sts %d (%s)\n", progname, tid, sts, strerror (sts));
        exit (EXIT_FAILURE);
    }
    
    // fetch information about the registers for this pid/tid
    if ((sts = devctl (fd, DCMD_PROC_GETGREG, &g, sizeof (g), NULL)) != EOK) {
        fprintf (stderr, "%s:  couldn't get GETGREG information, sts %d (%s)\n", progname, sts, strerror (sts));
        exit (EXIT_FAILURE);
    }

    printf ("\tInfo from DCMD_PROC_GETGREG\n");
    printf ("\t\tcs  0x%08X   eip  0x%08X\n", g.x86.cs, g.x86.eip);
    printf ("\t\tss  0x%08X   esp  0x%08X\n", g.x86.ss, g.x86.esp);
    printf ("\t\teax 0x%08X   ebx  0x%08X\n", g.x86.eax, g.x86.ebx);
    printf ("\t\tecx 0x%08X   edx  0x%08X\n", g.x86.ecx, g.x86.edx);
    printf ("\t\tedi 0x%08X   esi  0x%08X\n", g.x86.edi, g.x86.esi);
    printf ("\t\tebp 0x%08X   exx  0x%08X\n", g.x86.ebp, g.x86.exx);
    printf ("\t\tefl 0x%08X\n", g.x86.efl);
}

static void
dump_procfs_fpreg (int fd)
{
    printf ("\tInfo from DCMD_PROC_TIDSTATUS\n");

}

