#include "Structures.h"
/*������� ���������� �������� � ������ ����� ������-���� ��������.
�� �����:
- ��������� �� ������� struct node *q, ����� �������� ����� �������� �������
- �����, ������� ����� �������� � ������
�� ������:
- ��������� �� ����������� ������� struct node *p ��� NULL, ���� ��� �� ������� ��������
*/
struct node *add_node(struct node *q,int a)
{
	struct node *p;
	if((p=malloc(sizeof(struct node)))==NULL)
		return NULL;
	p->item=a;
	p->next=q->next;
	q->next=p;
	return p;
}
/*
�������, ����������� ������� � ������ ������.
�� �����:
- ��������� �� ������ ������� ������ struct node *first
- ������� int a, ������� ����� ��������
�� ������:
- ����� ������ ������ ������ struct node *p
*/
struct node *add_first(struct node *first,int a)
{
	struct node *p;
	if((p=malloc(sizeof(struct node)))==NULL)
		return first;
	p->item=a;
	p->next=first;
	return p;
}
/*
������� ���������� ������.
�� �����:
- ��������� �� ������ ������� ������ struct node *first
�� ������:
- ���-�� �������� ������
*/
int print_list(struct node *first)
{
	int i=0;
	struct node *p=first;
	while(p!=NULL)
	{
		printf("%d)\t%d\n",i+1,p->item);
		p=p->next;
		i++;
	}
	printf("\n");
	return i;
}
void free_list(struct node *first)
{
	struct node *t;
	struct node *p;
	p=first;
	if(p==NULL)
		return ;
	while((p->next) != NULL)
	{
		t=p;
		p=p->next;
		free(t);
	}
}
/*
�������, �������� � ������ ��� �������������� ����.
�� �����:
- ��������� �� ������ ������� ������ struct node *first
- ������ ����� int k � int l, ������� ����� �������� �������.
����������:
- �� �������� � ������ ��������� � �������� int k � int l ������������ ������ ������� ���.
*/
void swap_elements(struct node *first,int k,int l)
{
	int i;
	int temp;
	struct node *t;
	struct node *one;
	struct node *another;
	t=first;
	for(i=0;i<k-1;i++)
		t=t->next;
	one=t;
	t=first;
	for(i=0;i<l-1;i++)
		t=t->next;
	another=t;
	temp=one->item;
	one->item=another->item;
	another->item=temp;
}





