#include "Structures.h"
void elements_swap_interface(struct node *first)
{
	int k;
	int l;
	int n;
	printf("Your list was:\n");
	n=print_list(first);
	printf("Enter numbers of list's element which you want to swap\n");
	printf("The first element:\n");
	scanf("%d",&k);
	printf("The second element:\n");
	scanf("%d",&l);
	if(k>n || k<1 || l>n || l<1)
	{
		printf("There is no such elements in list.The list wasn't changed\n");
		return ;
	}
	swap_elements(first,k,l);
	printf("\n\n\n");
	printf("And now your list is:\n");
	print_list(first);
}