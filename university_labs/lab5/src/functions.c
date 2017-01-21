#include "Matrix.h"
int write_bin_matrix(char *fname, double **a,int n,int m)
{
	FILE *fp;
	fp=fopen(fname,"wb");
	if(fp==NULL)
		return 1;
	if(fwrite(&n,sizeof(int),1,fp)!=1)
		return 2;
	if(fwrite(&m,sizeof(int),1,fp)!=1)
		return 2;
	if(fwrite(*a,sizeof(double),n*m,fp)!= n*m)
		return 3;
	fclose(fp);
	return 0;
}
double **read_bin_matrix(char *fname, int *n, int *m)
{
	int i;
	double **a;
	FILE *fp;
	fp=fopen(fname,"rb");
	if(fp==NULL)
		return NULL;
	i=fread(n,sizeof(int),1,fp);
	if(i<1)
		goto err;
	i=fread(m,sizeof(int),1,fp);
	if(i<1)
		goto err;
	a=malloc_matrix((*n),(*m));
	if(a==NULL)
		goto err;
	i=fread(*a,sizeof(double),(*n)*(*m),fp);
	if(i<(*n)*(*m))
		goto err2;
	fclose(fp);
	return a;
err2: free_matrix(a);
err:  fclose(fp);
	  return NULL;
}
int write_txt_matrix(char *fname,double **a,int n,int m)
{
	int i,j;
	FILE *fp;
	fp=fopen(fname,"w");
	if(fp==NULL)
		return 1;
	fprintf(fp,"%d ",n);
	fprintf(fp,"%d \n",m);
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			fprintf(fp,"%lf\t",a[i][j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
}

double **read_txt_matrix(char *fname, int *n, int *m)
{
	int i,j;
	double **a;
	FILE *fp;
	fp=fopen(fname,"r");
	if(fp==NULL)
		return NULL;
	if(fscanf(fp,"%d",n)!=1)
		goto err;
	if(fscanf(fp,"%d",m)!=1)
		goto err;
	a=malloc_matrix((*n),(*m));
	if(a==NULL)
		goto err;
	for(i=0;i<(*n);i++)
		for(j=0;j<(*m);j++)
			if(fscanf(fp,"%lf",&(a[i][j]))!=1)
				goto err2;
	fclose(fp);
	return a;
err2: free_matrix(a);
err:  fclose(fp);
	  return NULL;
}

double **input_matrix(int n, int m,int lang)
{
	double **a;
	int i,j;
	a=malloc_matrix(n,m);
	if(a==NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
		{
			if(lang==1)
				printf("Enter the %d-th %d-th element of matrix\n",i+1,j+1);
			else
				printf("Введите [%d][%d]-ый элемент матрицы\n",i+1,j+1);

			scanf("%lf",&a[i][j]);
		}
	return a;
}
void output_matrix(double **a,int n, int m)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			fprintf(stdout,"%lf\t",a[i][j]);
		printf("\n");
	}
}

double **rnd_matrix(int n, int m)
{
	double **a;
	int i,j,s;
	s=time(NULL);
	srand(s);
	if((a=malloc_matrix(n,m))==NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
			a[i][j] = (rand()%100)/1.24;
	return a;
}
double **matrix_sum(double **a, double **b, int n, int m, int k, int l)
{
	int i,j;
	double **c;
	if(n!=k || m!= l)
		return NULL;
	if((c=malloc_matrix(n,m)) == NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
			c[i][j]=a[i][j]+b[i][j];
	return c;
}
double **matrix_multiplication(double **a, double **b, int n, int m, int k, int l)
{
	int i,j;
	double **c;
	if(m!= k)
		return NULL;
	if((c=malloc_matrix(n,l))==NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<l;j++)
		{
			c[i][j]=0;
			for(k=0;k<m;k++)
				c[i][j]+=a[i][k]*b[k][j];
		}
	return c;
}
/*
Функция выделения памяти для матрицы.
На входе:
- размерность матрицы int n и int m
На выходе:
- указатель на указатель на матрицу double **pa
Примечания:
- к элементам матрицы можно будет обращаться по их номерам a[i][j]
- память освобождается функцией free_matrix
*/
double **malloc_matrix(int n,int m)
{
	int i;
	double *a;
	double **pa;
	if(n==0 || m==0)
		return NULL;
	a=malloc(n*m*sizeof(double));
	if(a==NULL)
		return NULL;
	pa=malloc(n*sizeof(double*));
	if(pa==NULL)
	{
		free(a);
		return NULL;
	}
	for(i=0;i<n;i++)
		pa[i]=&a[i*m];
	return pa;
}
/*
Функция освобождения памяти матрицы типа double.
На входе:
- указатель double **pa на матрицу
*/
void free_matrix(int **pa)
{
	free(*pa);
	free(pa);
}
/*
Функция, меняющая местами элементы в столбце свободных членов.
На входе:
- массив свободных членов double *b
- номера элементов int k и int f, которые нужно переставить
- размер массива int n
Примечания:
- за наличием в массиве элементов k и f пользователь должен следить сам
*/
void swap_freerow(double *b,int k,int f)
{
	double t;
	t=b[k];
	b[k]=b[f];
	b[f]=t;
}
/*
Функция приведения матрицы типа double к треугольному виду методом Гаусса
На входе:
- матрица double **a
- размерность матрицы n
Примечания:
- программа работает только с квадратными матрицами
- за квадратностью матрицы пользователь должен следить сам
*/
int gauss_matrix(double **a, int n)
{
	double p;
	int i,j,k,t=0,l;
	if(n==1)
		return 0;
	for(k=0;k<n-1;k++)
	{
		for(i=k+1;i<n;i++)
		{
			if(fabs(a[k][k])<EPS)
				a[k][k]=0;
			if(a[k][k]==0)
			{
				t=1;
				for(l=k+1;l<n;l++)
					if(a[l][l]!=0)
						t=row_swap(a,k,l);
			}
			if(t==1)
				return 1;
			p=a[i][k]/a[k][k];
			for(j=k;j<n;j++)
				a[i][j]-=p*a[k][j];
		}
	}
	return 0;
}
/*
Функция, меняющая в матрице строки местами.
На входе:
- матрица double **a
- номера строк, меняющихся местами -  int k и int f
На выходе
- ноль
Примечания:
- За наличием строк с номерами k и f в матрице пользователь должен следить сам
*/
int row_swap(double **a, int k, int f)
{
	double *t;
	t=a[k];
	a[k]=a[f];
	a[f]=t;
	return 0;
}
/*
Функция решения СЛАУ методом Гаусса
На входе:
- матрица системы double **a
- столбец свободных членов double *b
- размерность матрицы и столбца свободных членов int n
Примечания:
- матрица системы должна быть квадратной
- размерность столбца свободных членов должна совпадать с размерностью матрицы системы
*/
int gauss_slau(double **a,double *b,int n)
{
	double p;
	int i,j,k,t,l;
	if(n==1)
		return 0;
	for(k=0;k<n-1;k++)
	{
		for(i=k+1;i<n;i++)
		{
			if(fabs(a[k][k])<EPS)
				a[k][k]==0;
			if(a[k][k]==0)
			{
				t=1;
				for(l=k+1;l<n;l++)
					if(a[l][l]!=0)
					{
						t=row_swap(a,k,l);
						swap_freerow(b,k,l);
					}
				if(t==1)
					return 1;
			}
			p=a[i][k]/a[k][k];
			for(j=k;j<n;j++)
				a[i][j]-=p*a[k][j];
			b[i]-=p*b[k];
		}
	}
	return 0;
}
/*
Функция нахождения решения СЛАУ обратным ходом метода Гаусса
На входе:
- уже приведённая к треугольному виу матрица double **a
- столбец свободных членов double *b, уже прошедший через прямой ход метода Гаусса
- размерность матрицы n
На выходе:
- массив решений double *x размерности n
Примечания:
- за соответствием параметров условиям, приведённым в начале описания, пользователь должен следить сам
*/
double *gauss_slau_reverse(double **a,double *b,int n)
{
	int j,k;
	double *x;
	x=calloc(n,sizeof(double));
	if(x==NULL)
		return NULL;
	x[n-1]=b[n-1]/a[n-1][n-1];
	for(k=n-2;k>=0;k--)
		x[k]=b[k]/a[k][k];
	for(k=n-2;k>=0;k--)
		for(j=k+1;j<n;j++)
			x[k]-=a[k][j]*x[j]/a[k][k];
	return x;
}




