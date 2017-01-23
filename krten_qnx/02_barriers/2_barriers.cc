#include <stdio.h>
#include <iostream>
#include <time.h>
#include <sync.h>
#include <sys/neutrino.h>
#include <unistd.h>

barrier_t barrier;
void * thread1 (void *not_used)
{
	time_t now;
	char buf[27];
	time(&now);
	std::cout << "Поток 1, время старта " << ctime_r (&now, buf);
	fflush(stdout);
	sleep(20);
	barrier_wait (&barrier);
	time(&now);
	std::cout << "Барьер в потоке 1, время срабатывания " << ctime_r (&now, buf);
	fflush(stdout);
	return 0;
}

void* thread2 (void *not_used)
{
	time_t now;
	char buf[27];
	time(&now);
	std::cout << "Поток 2, время старта " << ctime_r (&now, buf);
	fflush(stdout);
	sleep(40);
	barrier_wait (&barrier);
	time(&now);
	std::cout << "Барьер в потоке 2, время срабатывания " << ctime_r (&now, buf);
	fflush(stdout);

	return 0;
}

int main(void)
{
	time_t now;
	char buf[27];

	barrier_init (&barrier, NULL, 3);
	pthread_create (NULL, NULL, thread1, NULL);
	pthread_create (NULL, NULL, thread2, NULL);

	time (&now);
	std::cout << "main(): ожидание у барьера, время " << ctime_r (&now, buf);
	fflush(stdout);

	barrier_wait (&barrier);

	time(&now);
	std::cout << "Барьер в main, время срабатывания " << ctime_r (&now, buf);
	fflush(stdout);
	return 0;

}
