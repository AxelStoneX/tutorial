#include "Fifo.h"
int main(void)
{
	int a;
	int k;
	int er=0;
	struct queue *l;
	l=Fifo_creating();
	while(1)
	{
		printf("Enter the number\n1 - for element adding\n2 - for element deleting\n3 - for exit\n");
		scanf("%d",&k);
		if(k==1)
		{
			printf("Enter the element.Press Ctrl+z to end\n");

			while(scanf("%d",&a) != EOF)
			{
				if(Fifo_add(l,a)==1)
				{
					printf("Sorry\nThe queue is  full\nNow your queue is:\n");
					Fifo_print(l);
					printf("\n");
				}
				else
				{
					printf("Now your queue is:\n");
					Fifo_print(l);
					printf("\n");
				}
			}
		}
		if(k==2)
		{
			a=Fifo_deliting(l,&er);
			if(er==1)
				printf("Sorry!\nThe queue is empty\n");
			else
			{
				printf("Deleted elment is: %d\n",a);
				printf("Now your list is:\n");
				Fifo_print(l);
			}
				
		}
		if(k==3)
			break;
	}
	printf("Now yor queue is\n");
	Fifo_print(l);
	return 0;
}




