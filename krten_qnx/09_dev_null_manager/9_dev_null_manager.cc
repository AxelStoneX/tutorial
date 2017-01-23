#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

int main (int argc, char **argv)
{
    dispatch_t               *dpp;
    resmgr_attr_t            resmgr_attr;   // Управляющая структура
    resmgr_context_t         *ctp;          // Внутренний блок контекста
    resmgr_connect_funcs_t   connect_func;  // Таблица функций установления соединения
    resmgr_io_funcs_t        io_func;       // Таблица функций ввода-вывода
    iofunc_attr_t            attr;          // Атрибутная запись. Содержит информацию по каждому устройству

    // Создать структуру диспетчеризации
    if ((dpp = dispatch_create ()) == NULL)
    {
        perror ("Ошибка dispatch_create\n");
        exit (EXIT_FAILURE);
    }

    // Инициализировать структуры данных
    memset (&resmgr_attr, 0, sizeof (resmgr_attr));
    resmgr_attr.nparts_max = 1;
    resmgr_attr.msg_max_size = 2048;

    // Назначить вызовам обработчики по умолчанию
    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func, _RESMGR_IO_NFUNCS, &io_func);
    iofunc_attr_init (&attr, S_IFNAM | 0666, 0, 0);

    // Зарегестрировать префикс в пространстве имен путей
    if (resmgr_attach (dpp, &resmgr_attr, "/dev/mynull",
                       _FTYPE_ANY, 0, &connect_func, &io_func, &attr) == -1)
    {
        perror ("Ошибка resmgr_attach\n");
        exit (EXIT_FAILURE);
    }

    ctp = resmgr_context_alloc (dpp);

    //Ждать сообщений в вечном цикле
    while(1)
    {
        if ((ctp = resmgr_block (ctp)) == NULL)
        {
            perror ("Ошибка resmgr_block\n");
            exit (EXIT_FAILURE);
        }
        resmgr_handler (ctp);
    }
}
