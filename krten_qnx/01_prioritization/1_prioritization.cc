#include <iostream>
#include <sched.h>
#include <stdlib.h>
#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <sys/syspage.h>

void *new_thread_1(void *arg);
void *new_thread_2(void *arg);
void *new_thread_3(void *arg);

int main()
{
	int sched_rr;
	timespec interval;
	sched_rr = sched_rr_get_interval(94227, &interval);
	std::cout << "Интервал диспетчеризации: ";
	std::cout << interval.tv_sec << " с ";
	std::cout << interval.tv_nsec << " нс ";
	fflush(stdout);
	system("pwd");
	spawnl (P_WAIT, "/bin/ls", "/bin/ls", "-t", "-r", "-l", NULL);
	//execlp ( "ls", "ls", "-l", "-t", "-r", NULL );
	//int retval;

	std::cout << "Это определенно родительский процесс\n";
	fflush(stdout);
	//retval = fork();
	std::cout << "Кто это сказал?\n";

	std::cout << "PID родителя равен " << getpid() << std::endl;
	fflush(stdout);

	/* if (int child_pid = fork ())
		std::cout << "Это родитель, PID сына равен " << child_pid << std::endl;
	else
		std::cout << "Это сын, PID " << getpid(); */

	std::cout << "Значение по умолчанию области защиты потокового стека: ";
	std::cout << sysconf(_SC_PAGESIZE) << std::endl;

	//------------------------------------------------------------------------
	pthread_create(NULL, NULL, new_thread_1, NULL);
	pthread_create(NULL, NULL, new_thread_2, (void*) 123);

	pthread_attr_t attr;

	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
	attr.__param.__sched_priority = 15;
	pthread_create (NULL, &attr, new_thread_3, NULL);

	std::cout << "Число процессоров в системе: ";
	std::cout << _syspage_ptr -> num_cpu;

	return EXIT_SUCCESS;

	return 0;
}

void *new_thread_1(void *arg)
{
	std::cout << "Это new_thread_1\n";
	return 0;
}

void *new_thread_2(void *arg)
{
	std::cout << "Это new_thread_2\n";
	std::cout << (int) arg << std::endl;
	return 0;
}

void *new_thread_3(void *arg)
{
	std::cout << "Это new_thread_3\n";
	return 0;
}


