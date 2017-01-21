#include "TAS.h"
/*
Функция заполнения массива произвольной длины случайными числами.
На входе - размер массива.
На выходе - указатель на массив или NULL, если память не удалось выделить.
*/
int *rand_malloc_input_array(int n)
{
	int *a;
	int i;
	int s;
	if(n<0)
		return NULL;
	s=time(NULL);
	srand(s);
	a=malloc(n*sizeof(int));
	if(a==NULL)
		return NULL;
	for(i=0;i<n;i++)
		a[i]=rand()%100;
	return a;
}
void output_array(int n, int *a)
{
	int i;
	for(i=0;i<n;i++)
		printf("%d\t",a[i]);
}