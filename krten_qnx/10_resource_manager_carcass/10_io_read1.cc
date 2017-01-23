
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>

// Наша строка с данными
char  *data_string = "Здравствуй, мир!\n";

int io_read (resmgr_context_t *ctp, io_read_t *msg,
             iofunc_ocb_t *ocb)
{
    int      sts;            // Статус
    int      nbytes;
    int      nleft;
    int      off;            // Представление текущего смещения, которое мы должны будем использовать при обработке
    int      xtype;          // Сохранение xtype, содержавшегося в принятом сообщении
    struct   _xtype_offset *xoffset;

    // 1. Проверить, открыто ли устройство на чтение
    if ((sts = iofunc_read_verify (ctp, msg, ocb, NULL)) != EOK)
        return (sts);

    // 2. Проверить и обработать переопределение XTYPE
    xtype = msg -> i.xtype & _IO_XTYPE_MASK;
    if (xtype == _IO_XTYPE_OFFSET)
    {
        xoffset = (struct _xtype_offset *) (&msg -> i + 1);
        off = xoffset -> offset;
    } else if (xtype == _IO_XTYPE_NONE)
    {
        off = ocb -> offset;
    } else {
        return (ENOSYS);
    }

    // 3. Сколько байт осталось?
    nleft = ocb -> attr -> nbytes - off;

    // 4. Сколько байт мы можем отдать клиенту?
    nbytes = __min (nleft, msg -> i.nbytes);

    // 5. Если возвращаем данные, отдать их клиенту
    if (nbytes)
        MsgReply (ctp -> rcvid, nbytes, data_string + off, nbytes);

    // 6. Установить значение "atime" для POSIX stat()
    ocb -> attr -> flags |= IOFUNC_ATTR_ATIME | IOFUNC_ATTR_DIRTY_TIME;

    // 7. Если индекс lseek() не равен _IO_XTYPE_OFFSET
    //    увеличить его на число считанных байт
    if (xtype == _IO_XTYPE_NONE)
        ocb -> offset += nbytes;
    else
        // 8. Не возвращаем данные, просто разблокируем клиента
        MsgReply (ctp -> rcvid, EOK, NULL, 0);

    // 9. Сказать библиотеке, что мы уже ответили сами
    return (_RESMGR_NOREPLY);
}
