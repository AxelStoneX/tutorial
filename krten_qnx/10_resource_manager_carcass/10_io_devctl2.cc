#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <devctl.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <string.h>

#define DCMD_AUDIO_SET_SAMPLE_RATE  1
#define DCMD_AUDIO_GET_SAMPLE_RATE  2

int audio_set_samplerate2 (int samplerate)
{
    return 0;
}

int audio_get_samplerate2(void)
{
    return 0;
}

int io_devctl2 (resmgr_context_t *ctp, io_devctl_t *msg,
               iofunc_ocb_t *ocb)
{
    int sts;
    void *data;
    int nbytes;

    if ((sts = iofunc_devctl_default (ctp, msg, ocb)) != (int) _RESMGR_DEFAULT)
        return (sts);

    // 1. Установить указатель на область данных сообщения
    data = _DEVCTL_DATA (msg);

    // 2. Установить число возвращаемых байт в 0
    nbytes = 0;

    // Проверить все комнадыж покажем только те, которые нам интересны
    switch (msg -> i.dcmd )
    {
    // 3. Обработать команду SET
    case   DCMD_AUDIO_SET_SAMPLE_RATE:
        audio_set_samplerate2 (* (int *) data);
        break;

    // 4. Обработать команду GET
    case   DCMD_AUDIO_GET_SAMPLE_RATE:
        * (int *) data = audio_get_samplerate2();
        nbytes = sizeof (int);
        break;
    }

    // 5. Возвратить данные (если они есть) клиенту
    memset (&msg -> o, 0, sizeof (msg -> o));
    msg -> o.nbytes = nbytes;
    SETIOV (ctp -> iov, &msg -> o, sizeof (msg -> o) + nbytes);
    return (_RESMGR_NPARTS (1));
}
