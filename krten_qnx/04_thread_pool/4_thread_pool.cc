#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

const char *progname = "tp1";

void tag (const char *name)
{
	time_t t;
	char buffer [BUFSIZ];

	time(&t);
	strftime (buffer, BUFSIZ, "%T ", localtime (&t));
    printf ("%s %3d %-20.20s: ", buffer, pthread_self(), name);
}

THREAD_POOL_PARAM_T *
blockfunc (THREAD_POOL_PARAM_T *ctp)
{
	tag ("blockfunc"); printf("ctp %p\n", ctp);
	tag ("blockfunc"); printf("sleep (%d);\n", 15*pthread_self());
	sleep (pthread_self() * 15);
	tag ("blockfunc"); printf ("Выполнили sleep\n");
	tag ("blockfunc"); printf ("Возвращаем 0x%08X\n",
			0x10000000 + pthread_self());
	return ((resmgr_context_t *) (0x10000000 + pthread_self()));
}

THREAD_POOL_PARAM_T *
contextalloc (THREAD_POOL_HANDLE_T *handle)
{
	tag ("contextalloc"); printf ("handle %p\n", handle);
	tag ("contextalloc"); printf ("Возвращаем 0x%08X\n",
			0x20000000 + pthread_self());
	return ((resmgr_context_t *) (0x20000000 + pthread_self()));
}

void
contextfree (THREAD_POOL_PARAM_T *param)
{
	tag ("contextfree"); printf ("param %p\n", param);
}

void
unblockfunc (THREAD_POOL_PARAM_T *ctp)
{
	tag ("unblockfunc"); printf ("ctp %p\n", ctp);
}

int
handlerfunc (THREAD_POOL_PARAM_T *ctp)
{
    static int i = 0;

    tag ("handlerfunc"); printf ("ctp %p\n", ctp);
    if (i++ > 15)
    {
        tag ("handlerfunc"); printf ("Более 15 операций, возвращаем 0\n");
        return (0);
    }
    tag ("handlerfunc"); printf ("sleep (%d)\n", pthread_self () * 25);
    sleep (pthread_self () * 25);
    tag ("handlerfunc"); printf ("Выполнили sleep\n");
    tag ("handlerfunc"); printf ("Возвращаем 0x08X\n",
            0x30000000 + pthread_self());
    return (0x30000000 + pthread_self());
}

int main()
{
    thread_pool_attr_t tp_attr;
    void               *tpp;

    memset (&tp_attr, 0, sizeof (tp_attr));
    tp_attr.handle = (dispatch_t *) 0x12345678;
    tp_attr.block_func = blockfunc;
    tp_attr.unblock_func = unblockfunc;
    tp_attr.context_alloc = contextalloc;
    tp_attr.context_free = contextfree;
    tp_attr.handler_func = handlerfunc;

    tp_attr.lo_water = 3;
    tp_attr.hi_water = 7;
    tp_attr.increment = 2;
    tp_attr.maximum = 10;

    if ((tpp = thread_pool_create (&tp_attr,
            POOL_FLAG_USE_SELF)) == NULL)
    {
        fprintf (stderr, "%s: Ошибка thread_pool_create, errno %s\n", progname, strerror(errno));
        exit (EXIT_FAILURE);
    }

    thread_pool_start (tpp);

    fprintf (stderr, "%s: Возврат из Thread_pool_start; errno %s\n", progname, strerror (errno));
    sleep(3000);
    exit (EXIT_FAILURE);
}

