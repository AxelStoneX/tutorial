#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>

#define MT_WAIT_DATA    2
#define MT_SEND_DATA    3

#define CODE_TIMER      1

#define MT_OK           0
#define MT_TIMEOUT      1

typedef struct
{
    int messageType;
    int messageData;
} ClientMessageT;

typedef union
{
    ClientMessageT  msg;
    struct _pulse   pulse;
} MessageT;

#define MAX_CLIENT 16

extern "C"
{
struct
{
    int in_use;
    int rcvid;
    int timeout;
} clients [MAX_CLIENT];
}

int chid;
int debug = 1;
const char *progname = "time1.c";

static  void setupPulseAndTimer (void);
static  void gotAPulse (void);
static  void gotAMessage (int rcvid,
                          ClientMessageT *msg);
int main(void)
{
    int rcvid;
    MessageT msg;

    if ((chid = ChannelCreate (0)) == -1)
    {
        fprintf (stderr, "%s: не удалось создать канал!\n", progname);
        perror (NULL);
        exit (EXIT_FAILURE);
    }

    setupPulseAndTimer ();

    for (;;)
    {
        rcvid = MsgReceive (chid, &msg, sizeof(msg), NULL);

        if (rcvid == 0)
            gotAPulse();
        else
            gotAMessage (rcvid, &msg.msg);
    }

    return EXIT_SUCCESS;
}

void setupPulseAndTimer (void)
{
    timer_t  timerid;
    struct sigevent event;
    struct itimerspec timer;

    int coid;

    coid = ConnectAttach (0, 0, chid, 0, 0);
    if(coid == -1)
    {
        fprintf (stderr, "%s: ошибка ConnectAttach!\n", progname);
        perror(NULL);
        exit (EXIT_FAILURE);
    }

    SIGEV_PULSE_INIT (&event, coid,
                      SIGEV_PULSE_PRIO_INHERIT,
                      CODE_TIMER, 0);

    if (timer_create (CLOCK_REALTIME, &event, &timerid) == -1)
    {
        fprintf (stderr, "%s: не удалось создать таймер, errno %d\n", progname, errno);
        perror (NULL);
        exit (EXIT_FAILURE);
    }
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_nsec = 0;
    timer.it_interval.tv_nsec = 0;

    timer_settime (timerid, 0, &timer, NULL);
}

void gotAPulse (void)
{
    ClientMessageT msg;
    int            i;

    if (debug)
    {
        time_t now;
        time(&now);
        printf ("Получен импульс, время %s", ctime(&now));
    }

    msg.messageType = MT_TIMEOUT;

    for (i = 0; i < MAX_CLIENT; i++)
    {
        if (clients[i].in_use)
        {
            if (clients[i].timeout == 0)
            {
                MsgReply (clients[i].rcvid, EOK, &msg, sizeof (msg));
                clients[i].in_use = 0;
            }
        }
    }
}

void gotAMessage (int rcvid, ClientMessageT *msg)
{
    int i;
    switch (msg->messageType)
    {
    case   MT_WAIT_DATA:
        for(i = 0; i < MAX_CLIENT; i++)
        {
            if (!clients[i].in_use == 1)
            {
                clients[i].in_use = 1;
                clients[i].rcvid = rcvid;
                clients[i].timeout = 5;
                return;
            }
        }
        fprintf (stderr, "Таблица переполнена, сообщение от rcvid %d проигнорировано, клиент заблокирован\n", rcvid);
        break;
    case   MT_SEND_DATA:
        for (i = 0; i < MAX_CLIENT; i++)
        {
            if (clients[i].in_use)
            {
                msg->messageType = MT_OK;

                MsgReply (clients[i].rcvid, EOK, msg, sizeof (*msg));
                MsgReply (rcvid, EOK, msg, sizeof (*msg));
                clients[i].in_use = 0;
                return;
            }
        }
        fprintf (stderr, "Таблица пуста, сообщение от rcvid %d проигнорировано, клиент заблокирован\n", rcvid);
        break;
    }
}

