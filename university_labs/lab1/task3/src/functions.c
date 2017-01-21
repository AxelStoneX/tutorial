#include "TAS.h"
/*
Функция ввода массива произвольной длины со стандартного устройства ввода.
На входе - кол - во элементов массива.
На выходе - указатель на полученный массив или NULL в случае ошибки.
*/
int *malloc_input_array(int n)
{
	int i;
	int *a;
	a=malloc(n*sizeof(int));
	if(a==NULL)
		return NULL;
	for(i=0;i<n;i++)
	{
		printf("Enter the %d -th element of array\n",i);
		scanf("%d",&a[i]);
	}
	return a;
}
void output_array(int n, int *a)
{
	int i;
	for(i=0;i<n;i++)
		printf("%d\t",a[i]);
}