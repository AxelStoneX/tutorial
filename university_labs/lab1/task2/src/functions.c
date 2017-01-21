#include "TAS.h"
/*Функция чтения массива произвольной длины из текстового файла.
На входе - имя файла, указатель на переменную типа int, в которую
будет записано кол - во прочитанных символов.
Ны выходе - указатель на полученный массив. По адресу k будет записано кол-во элементов массива.
Память пользователь должен освободить сам.*/
int *readt_array(char *fname,int *k)
{
	int i,n,t;
	int *a;
	FILE *fp;
	fp=fopen(fname,"r");
	if(fp==NULL)
		return NULL;
	n=0;
	while(fscanf(fp,"%d",&t)==1)
		n++;
	a=malloc(n*sizeof(int));
	if(a==NULL)
	{
		fclose(fp);
		return a;
	}
	rewind(fp);
	for(i=0;i<n;i++)
		fscanf(fp,"%d",&a[i]);
	*k=n;
	fclose(fp);
	return a;
}
void output_array(int n, int *a)
{
	int i;
	for(i=0;i<n;i++)
		printf("%d\t",a[i]);
}