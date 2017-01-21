#include "Derivative.h"
int main(void)
{
	int l,t;
	printf("Select the operation\n1 - for derivating\n2 - for integrating\n3 - for root finding by Newthon's method\n4 - for root finding by bisection method\n");
	scanf("%d",&l);
	if(l==1)
		if((t=point1())==0)
			printf("Success!\n");
	if(l==2)
		if((t=point2())==0)
			printf("Success!\n");
	if(l==3)
		if((t=point3())==0)
			printf("Success!\n");
	if(l==4)
		if((t=point4())==0)
			printf("Success!\n");
	printf("Press any key to close the application\n");
	getch();
	return 0;
}

