#include "Fifo.h"
struct queue *Fifo_creating(void)
{
	int n;
	struct queue *l;
	l=malloc(sizeof(struct queue));
	printf("Enter number of elments in your queue\n");
	scanf("%d",&n);
	l->s=malloc(n*sizeof(int));
	l->n=0;
	l->max_n=n;
	l->head=0;
	l->tail=0;
	return l;
}
/*int Fifo_add(struct queue *l,int a)
{
	if(l->max_n == l->n)
		return 1;
	if(l->n == 0)
	{
		l->head=0;
		l->s[0]=a;
		l->n++;
	}
	if(l->n == 1)
	{
		l->tail=l->head+1;
		l->s[l->tail]=a;
		l->n=2;
	}
	if(l->tail > l->head)
		if(l->tail == (l->max_n-1))
		{
			l->s[0]=a;
			(l->n)++;
			l->tail=0;
		}
		else
		{
			l->s[(l->tail)]=a;
			(l->n)++;
			l->tail=(l->tail)+1;
		}
	if(l->tail < l->head)
	{
		l->s[l->tail+1]=a;
		(l->n)++;
	}
	/*if(l->tail==l->head)
	{
		l->s[0]=a;
		l->n=1;
		l->head=0;
		l->tail=1;
	}
	return 0;
}*/

/*
Функция добавления элемента в список(Голова слева).
На входе:
- очередь struct queue *l
- число int a, которое нужно добавить
На выходе:
- 1, если очередь заполнена
- 0, если элемент был добавлен успешно
*/
int Fifo_add(struct queue *l,int a)
{
	if(l->n==l->max_n)
		return 1;
	if(l->n==0)
	{
		l->head=0;
		l->tail=0;
		l->s[l->tail]=a;
		l->n=1;
		return 0;
	}
	if(l->tail >= l->head)
	{
		if(l->tail == (l->max_n)-1)
		{
			l->tail=0;
			l->s[l->tail]=a;
			l->n++;
			return 0;
		}
		else
		{
			l->tail++;
			l->s[l->tail]=a;
			l->n++;
			return 0;
		}
	}
	if(l->tail < l->head)
	{
		l->tail++;
		l->s[l->tail]=a;
		l->n++;
		return 0;
	}
	return 0;
}
void Fifo_print(struct queue *l)
{
	int i;
	if(l->n == 0)
	{
		printf("Your queue is empty\n");
		return ;
	}
	if(l->tail >= l->head)
		for(i=l->head;i<l->tail+1;i++)
			printf("%d ",l->s[i]);
	if(l->tail < l->head)
	{
		for(i=l->head; i<l->max_n; i++)
			printf("%d ",l->s[i]);
		for(i=0; i<l->tail+1; i++)
			printf("%d ",l->s[i]);
	}
	printf("\n");
}

/*Функция удаления первого элемента из очереди
На входе:
- очередь struct queue *l
- адрес int *er, по которому будет записан код ошибки
На выходе:
- "снятый" элемент типа int
- в er будет записана 1, если очередь пуста и удолять нечего
- в er будет записан 0 , если элемент удалён успешно
*/
int Fifo_deliting(struct queue *l, int *er)
{
	*er=0;
	if(l->n==0)
	{
		*er=1;
		return 0;
	}
	if(l->head == (l->max_n-1))
	{
		l->head=0;
		l->n--;
		return l->s[l->max_n-1];
	}
	l->head++;
	l->n--;
	return l->s[l->head-1];
}


	
