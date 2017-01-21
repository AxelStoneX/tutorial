#include "Derivative.h"
/*
Функция нахождения производной.
На входе
- точка x(double)
- шаг h(double)
- указатель на функцию double(*f)(double)
На выходе 
- значение производной в точке(double)
Примечания
- входные параметры пользователь должен прверять сам
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
/*Функция нахождения интеграла на прмежутке.
На входе: 
- границы интегрирования double a и double b
- точность (кол-во частей, на которые будет разбит итеграл) int n
- указатель на функцию double(*f)(double x)
На выходе:
- интегральная сумма double s
Примечания:
- входные параметры пользователь должен проверять сам
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
Функция поиска корней функции методом Ньютона.
На входе:
- границы интервала double a и double b
- точность double ex
- указатель на переменную, куда будет записан код ошибки int *er
- указатель на функцию double(*f)(double x)
На выходе:
- полученный корень
Коды ошибок:
1 - границы a и b неверны
2 - точность неверна
3 - неудача метода. Слишком много итераций цикла.
4 - неудача метода. Полученный корень оказался не в промежутке [ab]
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
Функция поиска корней функции на заданном промежутке методом бисекции.
На входе:
-  границы интервала double a и double b
- точность double ex
- указатель на переменную, куда будет записан отрицательный код ошибки int *er
- указатель на функцию double(*f)(double x)
На выходе:
- полученный корень double x в случае успеха
- 0 в случае ошибки и код ошибки, записанный в переменную er
Примчания:
- Функция возвращает кол-во затраченных итераций в переменную int *er в случае успеха
- Функция содержит константу MAX_ITERATION, показывающую максимально допустимое число итераций
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
