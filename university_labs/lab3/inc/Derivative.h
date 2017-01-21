#ifndef __DERIVATIVE_H__
#define __DERIVATIVE_H__

#define MAX_ITERATION 1000

#include <math.h>
#include <stdio.h>



double derivative(double x,double h,double(*f)(double x));

double integrating(double a, double b, int n, double(*f)(double x));

double root_finder(double a, double b, double ex, int *er, double(*f)(double x));

double f1(double x);

double f2(double x);

double f3(double x);

int point1(void);

int point2(void);

int point3(void);

int point4(void);

double root_finder_2(double a, double b, double ex, double *er, double(*f)(double x));

#endif //__DERIVATIVE_H__
