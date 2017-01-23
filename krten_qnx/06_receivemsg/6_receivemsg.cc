#include <sys/neutrino.h>
#include <stdio.h>
#include <iostream>
#include <process.h>
#include <errno.h>
using namespace std;

void server (void)
{
    int pid;
    int rcvid;
    int chid;
    char message [512];

    pid = getpid();
    chid = ChannelCreate (0);
    cout << "Создан канал, chid: " << chid << endl;;
    cout << "PID сервера: " << pid << endl;

    while(1)
    {
        rcvid = MsgReceive (chid, message, sizeof (message), NULL);
        printf ("Получил сообщение rcvid %X\n", rcvid);
        printf ("Сообщение такое: \"%s\".\n", message);

        strcpy (message, "Это ответ");
        MsgReply (rcvid, EOK, message, sizeof(message));
    }

}

int main(void)
{
    server();
    return 0;
}
