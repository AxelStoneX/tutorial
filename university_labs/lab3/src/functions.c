#include "Derivative.h"
/*
������� ���������� �����������.
�� �����
- ����� x(double)
- ��� h(double)
- ��������� �� ������� double(*f)(double)
�� ������ 
- �������� ����������� � �����(double)
����������
- ������� ��������� ������������ ������ �������� ���
*/
double derivative(double x,double h,double(*f)(double x))
{
	return (f(x+h)-f(x-h))/(h*2);
}
double f1(double x)
{
	return 4-x*x;
}
double f2(double x)
{
	return x*x*x-x*x*3+2*x;
}
double f3(double x)
{
	return sin(x);
}
/*������� ���������� ��������� �� ���������.
�� �����: 
- ������� �������������� double a � double b
- �������� (���-�� ������, �� ������� ����� ������ �������) int n
- ��������� �� ������� double(*f)(double x)
�� ������:
- ������������ ����� double s
����������:
- ������� ��������� ������������ ������ ��������� ���
*/
double integrating(double a, double b, int n, double(*f)(double x))
{
	int i;
	double h;
	double s=0;
	h=(b-a)/n;
	for(i=1;i<=n-1;i++)
		s+=h*(f(a+i*h));
	s+=(h/2)*(f(a)+f(b));
	return s;
}
/*
������� ������ ������ ������� ������� �������.
�� �����:
- ������� ��������� double a � double b
- �������� double ex
- ��������� �� ����������, ���� ����� ������� ��� ������ int *er
- ��������� �� ������� double(*f)(double x)
�� ������:
- ���������� ������
���� ������:
1 - ������� a � b �������
2 - �������� �������
3 - ������� ������. ������� ����� �������� �����.
4 - ������� ������. ���������� ������ �������� �� � ���������� [ab]
*/
double root_finder(double a, double b, double ex, int *er, double(*f)(double x))
{
	int n=0;
	double x;
	*er=0;
	if(a>b)
	{
		*er=1;
		return 0;
	}
	if(ex<0)
	{
		*er=2;
		return 0;
	}
	x=a;
	while(n<=1000 && abs(f(x))>ex)
	{
		x = x -(f(x)/derivative(x,ex,f));
		n++;
	}
	if(n>1000)
	{
		*er=3;
		return 0;
	}
	if(x<a && x>b)
	{
		*er=4;
		return 0;
	}
	return x;
}
/*
������� ������ ������ ������� �� �������� ���������� ������� ��������.
�� �����:
-  ������� ��������� double a � double b
- �������� double ex
- ��������� �� ����������, ���� ����� ������� ������������� ��� ������ int *er
- ��������� �� ������� double(*f)(double x)
�� ������:
- ���������� ������ double x � ������ ������
- 0 � ������ ������ � ��� ������, ���������� � ���������� er
���������:
- ������� ���������� ���-�� ����������� �������� � ���������� int *er � ������ ������
- ������� �������� ��������� MAX_ITERATION, ������������ ����������� ���������� ����� ��������
*/
double root_finder_2(double a, double b, double ex, double *er, double(*f)(double x))
{
	int n=0;
	double x;
	if(a>b)
	{
		*er=-1;
		return 0;
	}
	if(ex<0)
	{
		*er=-2;
		return 0;
	}
	x=(a+b)/2;
	while(fabs(f(x))>ex && n<MAX_ITERATION)
	{
		x=(a+b)/2;
		if(f(a)*f(x)<=0)
			b=x;
		else
			a=x;
		n++;
	}
	if(n>=MAX_ITERATION)
	{
		*er=-3;
		return 0;
	}
	if(x<a || x>b)
	{
		*er=-4;
		return 0;
	}
	*er=n;
	return x;
}
