#ifndef __FIFO
#define __FIFO

#define N 17
#define TIME 4320   //Трое суток - время работы аэропорта

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>


struct queue
{
	int *s;
	int n;
	int max_n;
	int head;
	int tail;
};

class Fifo
{
private:
	struct queue *l;
public:
	Fifo();
	Fifo(int n);
	~Fifo();
	int add(int a);
	void print(void);
	int deleting(int *er);
	int is_full(void);
	int is_empty(void);
	void head_change(int a);
	int get_n(void);
	int first(void);
	int sum(void);
};



#endif





