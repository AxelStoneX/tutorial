#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "myresmgr.h"

static resmgr_connect_funcs_t  connect_funcs;
static resmgr_io_funcs_t       io_funcs;
static iofunc_attr_t           attr;            // Атрибутная запись. Содержит информацию по каждому устройству

int main (int argc, char **argv)
{
    thread_pool_attr_t  pool_attr;
    thread_pool_t       *tpp;
    dispatch_t          *dpp;
    resmgr_attr_t       resmgr_attr;
    //resmgr_context_t    *ctp;
    int                 id;

    if ((dpp = dispatch_create ()) == NULL)
    {
        fprintf (stderr, "%s: Ошибка выделения контекста диспетчеризации\n", argv[0]);
        return (EXIT_FAILURE);
    }

    memset (&pool_attr, 0, sizeof (pool_attr));
    pool_attr.handle = dpp;
    pool_attr.context_alloc = resmgr_context_alloc;
    pool_attr.block_func = resmgr_block;
    pool_attr.handler_func = resmgr_handler;
    pool_attr.context_free = resmgr_context_free;

    // 1) Настроить пул потоков
    pool_attr.lo_water = 2;
    pool_attr.hi_water = 4;
    pool_attr.increment = 1;
    pool_attr.maximum = 50;
    if ((tpp = thread_pool_create (&pool_attr, POOL_FLAG_EXIT_SELF)) == NULL)
    {
        fprintf (stderr, "%s: Ошибка инициализации пула потоков\n", argv[0]);
        return (EXIT_FAILURE);
    }

    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_funcs,
                      _RESMGR_IO_NFUNCS, &io_funcs);

    io_funcs.read = io_read;
    io_funcs.write = io_write;
    io_funcs.devctl = io_devctl;

    iofunc_attr_init (&attr, S_IFNAM | 0777, 0, 0);

    // 2) Переопределить функции установления соединения
    // и функции ввода/вывода, как надо

    memset (&resmgr_attr, 0, sizeof (resmgr_attr));
    resmgr_attr.nparts_max = 1;
    resmgr_attr.msg_max_size = 2048;

    // 3) Замените "/dev/whatever" на нужный префикс
    if ((id = resmgr_attach (dpp, &resmgr_attr, "/dev/whatever",
                             _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &attr)) == -1)
    {
        fprintf (stderr, "%s: Ошибка регистрации префикса\n", argv [0]);
        return (EXIT_FAILURE);
    }

    // Отсюда возврата уже не будет
    thread_pool_start (tpp);
}
