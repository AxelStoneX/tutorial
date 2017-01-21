#include "Fifo.h"
/*
����������� ������� �� ���������.
������ ������� �� N ���� ��� ���� int
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
��������������� ����������� �������.
������ ������� �� n ���� ���� int
�� �����:
- ���-�� ���� � ������� int n
����������:
- ������������ ��� ������ ������� �� ���, ����� n ���� ������ ����
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
���������� �������.
���������� - �� � � ������ ����������.
*/
Fifo::~Fifo()
{
	delete l;
}

/*
������� ���������� �������� � �������.
�� �����:
- ������� int a
�� ������:
- 0, ���� �� ������ �������
- 1, ���� ������� ��������� � ������� �������� �� �������.
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
������� ������ ������� �� �����
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
������� �������� ������� �������� �� �������
�� �����:
- ����� int *er, �� �������� ����� ������� ��� ������
�� ������:
- "������" ������� ���� int
- � er ����� �������� 1, ���� ������� ����� � ������� ������
- � er ����� ������� 0 , ���� ������� ����� �������
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
������� �������� ������� �� �������������
�� ������:
- 1, ���� ������� ����������
- 2, ���� ������� �����
*/
int Fifo::is_full(void)
{
	if(l->n==l->max_n)
		return 1;
	else
		return 0;
}
/*
������� �������� ������� �� �������
�� ������:
- 1, ���� ������� �����
- 2, ���� � �� ���-�� ����
*/
int Fifo::is_empty(void)
{
	if(l->n==0)
		return 1;
	else
		return 0;
}
/*
������� ��������� ������� �������� �������
�� �����:
- ����� int a, ������������ � ������� ��������
*/
void Fifo::head_change(int a)
{
	l->s[l->head]+=a;
}

/*
�������, ������������ ���-�� ��������� � �������
*/
int Fifo::get_n(void)
{
	return l->n;
}
/*
�������, ������������ ������ ������� ������
*/
int Fifo::first(void)
{
	return l->s[l->head];
}
/*
������� �������� ����� ��������� � �������
�� ������:
- ����� ��������� � ������� int sum
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
