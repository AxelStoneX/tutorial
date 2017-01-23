#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int data_ready = 0;
pthread_mutex_t mutex  = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;

void* consumer (void *notused)
{
	cout << "Это поток-потребитель\n";
	while(1)
	{
		pthread_mutex_lock (&mutex);
		while (!data_ready)
			pthread_cond_wait (&condvar, &mutex);
		cout << "Потребитель получил данные от производителя\n";
		data_ready = 0;
		pthread_cond_signal (&condvar);
		pthread_mutex_unlock (&mutex);
	}
	return 0;
}

void *producer (void *notused)
{
	cout << "Это поток-производитель\n";
	while(1)
	{
		sleep(1);
		cout << "Производитель получил данные от h/w\n";
		pthread_mutex_lock (&mutex);
		while (data_ready)
			pthread_cond_wait(&condvar, &mutex);
		data_ready = 1;
		pthread_cond_signal (&condvar);
		pthread_mutex_unlock (&mutex);
	}
	return 0;
}

int main(void)
{
	cout << "Начало примера с производителем и потребителем\n";
	pthread_create (NULL, NULL, producer, NULL);
	pthread_create (NULL, NULL, consumer, NULL);
	sleep(20);
}
