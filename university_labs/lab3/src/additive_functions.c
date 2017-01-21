#include "Derivative.h"
double (*pf[3])(double x)={f1,f2,f3};
int point1(void)
{
	int k;
	double a;
	double b;
	double h;
	double x;
	printf("Enter the number\n1 - for x^2\n2 - for x^3-3(x^2)+2x\n3 - for sin(x)*cos(x)\n");
	scanf("%d",&k);
	if(k!=1 && k!=2 && k!=3)
	{
		printf("You pressed wrong key\n");
		return 1;
	}
	printf("Enter low limits of derivating\n");
	scanf("%lf",&a);
	printf("Enter high limit of derivating\n");
	scanf("%lf",&b);
	if(b<a)
	{
		printf("Limits of derivating don't match\n");
		return 2;
	}
	printf("Enter step of derivating\n");
	scanf("%lf",&h);
	for(x=a;x<=b;x+=h)
	{
		printf("f(%lf) = %lf\t",x,pf[k-1](x));
		printf("f' = %lf\n",derivative(x,h,pf[k-1]));
	}
	return 0;
}
int point2(void)
{
	int k,n;
	double a;
	double b;
	printf("Enter the number\n1 - for x^2\n2 - for x^3-3(x^2)+2x\n3 - for sin(x)*cos(x)\n");
	scanf("%d",&k);
	if(k!=1 && k!=2 && k!=3)
	{
		printf("You pressed wrong key\n");
		return 3;
	}
	printf("Enter the lower limit of integrating\n");
	scanf("%lf",&a);
	printf("Enter the higher limit of integrating\n");
	scanf("%lf",&b);
	if(b<a)
	{
		printf("Limits of derivating don't match\n");
		return 4;
	}
	printf("Enter the exactility of method\n(number of parts, which will be split function)\n");
	scanf("%d",&n);
	if(n<0)
	{
		printf("Wrong exactility\n");
		return 5;
	}
	printf("The integral is %lf",integrating(a,b,n,pf[k-1]));
	return 0;
}
int point3(void)
{
	int k;
	int er=0;
	double a;
	double b;
	double ex;
	double r;
	printf("Enter the number\n1 - for x^2\n2 - for x^3-3(x^2)+2x\n3 - for sin(x)*cos(x)\n");
	scanf("%d",&k);
	if(k!=1 && k!=2 && k!=3)
	{
		printf("You pressed wrong key\n");
		return 6;
	}
	printf("Enter the lower limit of root finding\n");
	scanf("%lf",&a);
	printf("Enter the higher limit of root finding\n");
	scanf("%lf",&b);
	printf("Enter the exactility\n");
	scanf("%lf",&ex);
	r=root_finder(a,b,ex,&er,pf[k-1]);
	if(er==1)
	{
		printf("Limits of derivating don't match\n");
		return 7;
	}
	if(er==2)
	{
		printf("Wrong exactility\n");
		return 8;
	}
	if(er==3 || er==4)
	{
		printf("Error of Newton's method. Impossible to find a root.\n");
		return 9;
	}
	printf("The root of selected function is %lf\n",r);
	return 0;
}
int point4(void)
{
	int k;
	int er = 0;
	double a;
	double b;
	double ex;
	double r;
	printf("Enter the number\n1 - for 4-x^2\n2 - for x^3-3(x^2)+2x\n3 - for sin(x)\n");
	scanf("%d",&k);
	if(k!=1 && k!=2 && k!=3)
	{
		printf("You pressed wrong key\n");
		return 10;
	}
	printf("Enter the lower limit of root finding\n");
	scanf("%lf",&a);
	printf("Enter the higher limit of root finding\n");
	scanf("%lf",&b);
	printf("Enter the exactility\n");
	scanf("%lf",&ex);
	r=root_finder_2(a,b,ex,&er,pf[k-1]);
	if(er==-1)
	{
		printf("Limits of derivating don't match\n");
		return 11;
	}
	if(er==-2)
	{
		printf("Wrong exactility\n");
		return 12;
	}
	if(er==-3 || er==-4)
	{
		printf("Error of bisection method. Impossible to find a root.\n");
		return 13;
	}
	printf("f(%lf)= %lf\nThe root of selected function is %lf\nIterations: %d\n",r,pf[k-1](r),r,er);
	return 0;
}

	






