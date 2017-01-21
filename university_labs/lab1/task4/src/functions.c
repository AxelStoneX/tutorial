#include "TAS.h"
/*
������� ���������� ������� ������������ ����� ���������� �������.
�� ����� - ������ �������.
�� ������ - ��������� �� ������ ��� NULL, ���� ������ �� ������� ��������.
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