#include "Fifo.h"
/*
Конструктор очереди по умолчанию.
Создаёт очередь на N мест для типа int
*/
Fifo::Fifo()
{
	l=new struct queue;
	l->s=new int[N];
	l->n=0;
	l->max_n=N;
	l->head=0;
	l->tail=0;
}

/*
Параметрический конструктор очереди.
Создаёт очередь на n мест типа int
На входе:
- кол-во мест в очереди int n
Примечание:
- пользователь сам должен следить за тем, чтобы n было больше нуля
*/
Fifo::Fifo(int n)
{
	l=new struct queue;
	l->s=new int[n];
	l->n=0;
	l->max_n=n;
	l->head=0;
	l->tail=0;
}

/*
Деструктор очереди.
Деструктор - он и в Африке деструктор.
*/
Fifo::~Fifo()
{
	delete l;
}

/*
Функция добавления элемента в очередь.
На входе:
- элемент int a
На выходе:
- 0, если всё прошло успешно
- 1, если очередь заполнена и элемент добавить не удалось.
*/
int Fifo::add(int a)
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

/*
Функция вывода очереди на экран
*/
void Fifo::print(void)
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

/*
Функция удаления первого элемента из очереди
На входе:
- адрес int *er, по которому будет записан код ошибки
На выходе:
- "снятый" элемент типа int
- в er будет записана 1, если очередь пуста и удалять нечего
- в er будет записан 0 , если элемент удалён успешно
*/
int Fifo::deleting(int *er)
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

/*
Функция проверки очереди на заполненность
На выходе:
- 1, если очередь заполненна
- 2, если очередь пуста
*/
int Fifo::is_full(void)
{
	if(l->n==l->max_n)
		return 1;
	else
		return 0;
}
/*
Функция проверки очереди на пустоту
На выходе:
- 1, если очередь пуста
- 2, если в неё что-то есть
*/
int Fifo::is_empty(void)
{
	if(l->n==0)
		return 1;
	else
		return 0;
}
/*
Функция изменения первого элемента очереди
На входе:
- число int a, прибавляемое к первому элементу
*/
void Fifo::head_change(int a)
{
	l->s[l->head]+=a;
}

/*
Функция, возвращающая кол-во элементов в очереди
*/
int Fifo::get_n(void)
{
	return l->n;
}
/*
Функция, возвращающая первый элемент списка
*/
int Fifo::first(void)
{
	return l->s[l->head];
}
/*
Функция подсчёта суммы элементов в очереди
На выходе:
- сумма элементов в очереди int sum
*/
int Fifo::sum(void)
{
	int i;
	int sum=0;
	if(l->n==0)
		return 0;
	if(l->head < l->tail)
		for(i=l->head;i<l->tail;i++)
			sum+=l->s[i];
	if(l->head > l->tail)
	{
		for(i=l->head;i<l->max_n;i++)
			sum+=l->s[i];
		for(i=0;i<l->tail;i++)
			sum+=l->s[i];
	}
	return sum;
}
