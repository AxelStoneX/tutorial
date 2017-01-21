#include "Structures.h"
/*Функция добавления элемента в список после какого-либо элемента.
На входе:
- указатель на элемент struct node *q, после которого нужно добавить элемент
- Число, которое нужно вставить в список
На выходе:
- указатель на добавленный элемент struct node *p или NULL, если его не удалось вставить
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
Функция, добавляющая элемент в начало списка.
На входе:
- указатель на первый элемент списка struct node *first
- элемент int a, который нужно вставить
На выходе:
- новое начало начало списка struct node *p
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
Функция распечатки списка.
На входе:
- указатель на первый элемент списка struct node *first
На выходе:
- кол-во элеметов списка
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
Функция, меняющая в списке два информационных поля.
На входе:
- указатель на первый элемент списка struct node *first
- номера полей int k и int l, которые нужно поменять местами.
Примечания:
- за наличием в списке элементов с номерами int k и int l пользователь должен следить сам.
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





