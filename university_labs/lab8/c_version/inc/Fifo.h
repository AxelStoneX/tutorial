#include <stdio.h>
struct queue
{
	int *s;
	int n;
	int max_n;
	int head;
	int tail;
};

struct queue *Fifo_creating(void);

int Fifo_add(struct queue *l,int a);

void Fifo_print(struct queue *l);

int Fifo_deliting(struct queue *l, int *er);