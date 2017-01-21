#include "TAS.h"

/* Функция записи массива произвольной длины в текстовый файл.
На входе - имя файла, указатель на первый элемент массива и кол - во элементов.
На выходе - 0, если всё успешно, 1, если файл не был открыт. */

int writet_array(char *fname, int a[MAX_LEN], int n)
{
	FILE *fp;
	int i;
	if((fp=fopen(fname,"w"))==NULL)
		return 1;
	for(i=0;i<n;i++)
		fprintf(fp,"%d ",a[i]);
	fclose(fp);
	return 0;
}
int input_array(int n,int a[MAX_LEN])
{
	int i;
	if(n<0 || n>MAX_LEN)
		return 1;
	for(i=0;i<n;i++)
	{
		printf("Enter %d -th element of array\n",i);
		scanf("%d",&a[i]);
	}
	return 0;
}
