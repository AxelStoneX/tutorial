#include "Matrix.h"
int main(void)
{
	int n,m,k,l,r,u;
	int g;
	float *a,*b,*c;
	char fname[MAX_LEN];
	//Запрос первой матрицы
	printf("Enter the first marix\n");
	printf("What kind of matrix input you want to use?\n1 - standart input\n2 - randomize input\n3 - txt input\n4 - binary input\n");
	scanf("%d",&g);
	if(g==1)
	{
		printf("Enter number of the rows\n");
		scanf("%d",&n);
		printf("Enter number of the columns\n");
		scanf("%d",&m);
		if((a=input_matrix(n,m))==NULL)
		{
			printf("There was some errors with memory allocating\n");
			return 1;
		}
	}
	if(g==2)
	{
		printf("Enter number of the rows\n");
		scanf("%d",&n);
		printf("Enter number of the columns\n");
		scanf("%d",&m);
		if((a=rnd_matrix(n,m))==NULL)
		{
			printf("There was some errors with memory allocating\n");
			return 2;
		}
	}
	if(g==3)
	{
		printf("Enter name of the file\n");
		scanf("%s",fname);
		if((a=read_txt_matrix(fname,&n,&m))==NULL)
		{
			printf("There was some errors with txt-file opening or memory allocating\n");
			return 3;
		}
	}
	if(g==4)
	{
		printf("Enter name of the file\n");
		scanf("%s",fname);
		if((a=read_bin_matrix(fname,&n,&m))==NULL)
		{
			printf("There was some errors with txt-file opening or memory allocating\n");
			return 4;
		}
	}
	if(g!=1 && g!=2 && g!=3 && g!=4)
	{
		printf("You pressed wrong key\n");
		return 7;
	}
	//Запрос второй матрицы
	printf("Enter the second matrix\n");
	printf("What kind of matrix input you want to use?\n1 - standart input\n2 - randomize input\n3 - txt input\n4 - binary input\n");
	scanf("%d",&g);
	if(g==1)
	{
		printf("Enter number of the rows\n");
		scanf("%d",&k);
		printf("Enter number of the columns\n");
		scanf("%d",&l);
		if((b=input_matrix(k,l))==NULL)
		{
			printf("There was some errors with memory allocating\n");
			free(a);
			return 1;
		}
	}
	if(g==2)
	{
		printf("Enter number of the rows\n");
		scanf("%d",&k);
		printf("Enter number of the columns\n");
		scanf("%d",&l);
		if((b=rnd_matrix(k,l))==NULL)
		{
			printf("There was some errors with memory allocating\n");
			free(a);
			return 2;
		}
	}
	if(g==3)
	{
		printf("Enter name of the file\n");
		scanf("%s",fname);
		if((b=read_txt_matrix(fname,&k,&l))==NULL)
		{
			printf("There was some errors with txt-file opening or memory allocating\n");
			free(a);
			return 3;
		}
	}
	if(g==4)
	{
		printf("Enter name of the file\n");
		scanf("%s",fname);
		if((b=read_bin_matrix(fname,&k,&l))==NULL)
		{
			printf("There was some errors with txt-file opening or memory allocating\n");
			free(a);
			return 4;
		}
	}
	if(g!=1 && g!=2 && g!=3 && g!=4)
	{
		printf("You pressed wrong key\n");
		free(a);
		return 7;
	}
	//Запрос сложения или вычитания матриц
	printf("What do you want to do with matrxes?\n1 - sum\n2 - multiplicate\n");
	scanf("%d",&g);
	if(g==1)
	{
		if((c=matrix_sum(a,b,n,m,k,l))==NULL)
		{
			printf("There was error with matrix sum finding\n");
			free(a);
			free(b);
			return 5;
		}
		r=n;
		u=m;
	}
	if(g==2)
	{
		if((c=matrix_multiplication(a,b,n,m,k,l))==NULL)
		{
			printf("There was errors with matrixes multiplication\n");
			free(a);
			free(b);
			return 6;
		}
		r=n;
		u=l;
	}
	if(g!=1 && g!=2)
	{
		printf("You pressed wrong key\n");
		free(a);
		free(b);
		return 7;
	}
	//Запрос вывода результата
	printf("What kind of output do you want to use?\n1 - standart output\n2 - txt-file output\n3 - bin-file output\n");
	scanf("%d",&g);
	if(g==1)
		output_matrix(c,r,u);
	if(g==2)
	{
		printf("Enter name of the file\n");
		scanf("%s",fname);
		if(write_txt_matrix(fname,c,r,u)!=0)
		{
			printf("There was some errors with file opening\n");
			free(a);
			free(b);
			free(c);
			return 8;
		}
	}
	if(g==3)
	{
		printf("Enter name of the file\n");
		scanf("%s",fname);
		if(write_bin_matrix(fname,c,r,u)!=0)
		{
			printf("There was some errors with file opening\n");
			free(a);
			free(b);
			free(c);
			return 9;
		}
	}
	free(a);
	free(b);
	free(c);
	return 0;
}






