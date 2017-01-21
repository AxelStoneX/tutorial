#include <stdio.h>
struct node
{
	int item;
	struct node *next;
};

struct node *add_node(struct node *q,int a);

struct node *add_first(struct node *first,int a);

int print_list(struct node *first);

void elements_swap_interface(struct node *first);

void swap_elements(struct node *first,int k,int l);