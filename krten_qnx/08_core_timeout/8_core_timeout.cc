#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <unistd.h>

#define SEC_NSEC 1000000000LL;

void * long_thread (void *notused)
{
    printf ("Этот поток выполняется более 10 секунд\n");
    sleep(20);
    return 0;
}

int main(void)
{
    uint64_t   timeout;
    struct     sigevent event;
    int        rval;
    pthread_t  thread_id;

    SIGEV_UNBLOCK_INIT (&event);
    pthread_create (&thread_id, NULL, long_thread, NULL);
    timeout = 10LL * SEC_NSEC;

    TimerTimeout (CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);

    rval = pthread_join (thread_id, NULL);
    if (rval == ETIMEDOUT)
        printf ("Истекли 10 секунд ожидания, поток %d все еще выполняется!\n", thread_id);
    sleep(5);
    TimerTimeout (CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    rval = pthread_join (thread_id, NULL);
    if (rval == ETIMEDOUT)
        printf ("Истекли 25 секунд, поток %d все еще выполняется (нехорошо)!\n", thread_id);
    else
        printf ("Поток %d завершен (как и ожидалось!)\n", thread_id);
    return 0;
}
