#include "Structures.h"
int main(void)
{
	int k=0;
	int n;
	int a;
	int i=1;
	struct node *first=NULL;
	struct node *last=NULL;
	printf("Enter the list. If you want to exit,press Ctrl+Z\n");
	printf("%d)\t",i);
	k=scanf("%d",&a);
	if(k==-1)
	{
		printf("Your list is empty\n");
		return 1;
	}
	if((first=add_first(first,a))==NULL)
	{
		printf("There was some errors with memeory allocating for first element\n");
		return 2;
	}
	i++;
	last=first;
	while(k!=-1)
	{
		printf("%d)\t",i);
		k=scanf("%d",&a);
		if(k==-1)
			break;
		if((last=add_node(last,a))==NULL)
		{
			k=-1;
			break;
		}
		i++;
	}
	printf("\n\n\n");
	n=print_list(first);
	printf("Elements in list:\t%d\n",n);
	while(1)
	{
		printf("What do you want to do\n1 - for elements swap\n2 - for exit application\n");
		scanf("%d",&k);
		if(k==1)
			elements_swap_interface(first);
		if(k==2)
		{
			printf("The application will be closed.Press any key\n");
			free_list(first);
			getch();
			return 0;
		}
		if(k!=1 && k!= 2)
			printf("You pressed wrong key.Try again\n");
	}
	free_list(first);
	return 0;
}
