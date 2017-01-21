#include "Matrix.h"
/*
Функция запроса ввода матрицы.
На входе:
- матрица типа double **a
- язык интерфейса int lang(1 - английский,2 - русский)
На выходе:
- матрица типа double **a или NULL в случае ошибок
- размерность n и m в неявном виде
Примечания:
- память пользователь должен освобождать сам функцией free_matrix
- язык по умолчанию - русский
*/
double **matrix_input_interface(int *n, int *m, int lang)
{
	double **a;
	int g;
	char fname[MAX_LEN];
	setlocale( LC_CTYPE, ".1251" );
	if(lang==1)
		printf("What kind of matrix input you want to use?\n1 - standart input\n2 - randomize input\n3 - txt input\n4 - binary input\n");
	else
		printf("Какой вид ввода матрицы вы хотите использовать?\n1 - для ввода с клавиатуры\n2 - для случайной матрицы\n3 - для чтения из текстового файла\n4 - для чтения из двоичного файла\n");
	scanf("%d",&g);
	printf("\n");
	if(g==1)
	{
		if(lang==1)
			printf("Enter count of matrix' rows\n");
		else
			printf("Введите кол-во строк в вашей матрице\n");
		scanf("%d",n);
		if(lang==1)
			printf("Enter count of matrix' columns\n");
		else
			printf("Введите кол-во столбцов вашей матрицы\n");
		scanf("%d",m);
		if((a=input_matrix(*n,*m,lang))==NULL)
		{
			if(lang==1)
				printf("There was some errors with memory allocating\n");
			else
				printf("Ошибка выделения памяти под матрицу\n");
			getch();
			return NULL;
		}
	}
	if(g==2)
	{
		if(lang==1)
			printf("Enter count of matrix' rows\n");
		else
			printf("Введите кол-во строк в вашей матрице\n");
		scanf("%d",n);
		if(lang==1)
			printf("Enter count of matrix' columns\n");
		else
			printf("Введите кол-во столбцов вашей матрицы\n");
		scanf("%d",m);
		if((a=rnd_matrix(*n,*m))==NULL)
		{
			if(lang==1)
				printf("There was some errors with memory allocating\n");
			else
				printf("Ошибка выделения памяти под матрицу\n");
			getch();
			return NULL;
		}
	}
	if(g==3)
	{
		if(lang==1)
			printf("Enter name of the file\n");
		else
			printf("Введите имя файла\n");
		scanf("%s",fname);
		if((a=read_txt_matrix(fname,n,m))==NULL)
		{
			if(lang==1)
				printf("There was some errors with txt-file opening or memory allocating\n");
			else
				printf("Возникли ошибки с чтением текстового файла или выделением памяти\n");
			getch();
			return NULL;
		}
	}
	if(g==4)
	{
		if(lang==1)
			printf("Enter name of the file\n");
		else
			printf("Введите имя файла\n");
		scanf("%s",fname);
		if((a=read_bin_matrix(fname,n,m))==NULL)
		{
			if(lang==1)
				printf("There was some errors with txt-file opening or memory allocating\n");
			else
				printf("Воникли ошибки с чтением двоичного файла или выделением памяти\n");
			getch();
			return NULL;
		}
	}
	if(g!=1 && g!=2 && g!=3 && g!=4)
	{
		if(lang==1)
			printf("You pressed wrong key\n");
		else
			printf("Вы нажали не ту кнопку\n");
		getch();
		return NULL;
	}
	return a;
}
/*
Функция запроса вывода матрицы
На входе:
- матрица типа double **a
- размерность матрицы int n и int m
- язык интерфейса int lang(1 - английский,2 - русский)
На выходе:
- код ошибки или 0 в случае успеха вывода матрицы
Коды ошибок:
1 - в случае невозможности открыть текстовый файл
2 - в случае ошибок с открытием бинарного файла
Примечания:
- язык по умолчанию - русский
*/
int matrix_output_interface(double **a,int n,int m,int lang)
{
	char fname[MAX_LEN];
	int g;
	setlocale( LC_CTYPE, ".1251" );
	if(lang==1)
		printf("What kind of output do you want to use?\n1 - standart output\n2 - txt-file output\n3 - bin-file output\n");
	else
		printf("Какой вид вывода вы хотите использовать?\n1 - для вывода на экран\n2 - для записи в текстовый файл\n3 - для записи в двоичный файл\n");
	scanf("%d",&g);
	printf("\n");
	if(g==1)
		output_matrix(a,n,m);
	if(g==2)
	{
		if(lang==1)
			printf("Enter name of the file\n");
		else
			printf("Введите имя файла\n");
		scanf("%s",fname);
		if(write_txt_matrix(fname,a,n,m)!=0)
		{
			if(lang==1)
				printf("There was some errors with file opening\n");
			else
				printf("Возникла ошибка записи в файл\n");
			getch();
			return 1;
		}
	}
	if(g==3)
	{
		if(lang==1)
			printf("Enter name of the file\n");
		else
			printf("Введите имя файла\n");
		scanf("%s",fname);
		if(write_bin_matrix(fname,a,n,m)!=0)
		{
			if(lang==1)
				printf("There was some errors with file opening\n");
			else
				printf("Возникла ошибка записи в файл\n");
			getch();
			return 1;
		}
	}
	return 0;
}
/*
Функция запроса решения СЛАУ
На входе 
- матрица СЛАУ double **a без столбца свободных членов
- размерность матрицы n
- язык интерфейса int lang(1 - английский,2 - русский)
На выходе:
- массив решений double *x или NULL в случае ошибок
Примечания:
- за квадратностью матрицы пользователь долен следить сам
- язык по умолчанию - русский
*/
double *gauss_interface(double **a,int n,int lang)
{
	double *b;
	double *x;
	int i;
	setlocale( LC_CTYPE, ".1251" );
	b=malloc(n*sizeof(double));
	if(b==NULL)
		return NULL;
	if(lang==1)
		printf("Enter the column of free members\n");
	else
		printf("Введите столбец свободных членов\n");
	for(i=0;i<n;i++)
	{
		scanf("%lf",&b[i]);
		printf("\n");
	}
	if(gauss_slau(a,b,n)!=0)
	{
		if(lang==1)
			printf("There was some errors with SLAU solving\n");
		else
			printf("Возникли ошибки с поиском решения СЛАУ. Возможно, система не имеет единственного решения\n");
		getch();
		return NULL;
	}
	x=gauss_slau_reverse(a,b,n);
	if(x==NULL)
	{
		free(b);
		if(lang==1)
			printf("There was some errors with solve finding\n");
		else
			printf("Возникли ошибки с поиском решения\n");
		getch();
		return NULL;
	}
	if(lang==1)
		printf("There is answer:\n");
	else
		printf("А вот и ответ:\n");
	for(i=0;i<n;i++)
		printf("x[%d]:\t %lf\n",i+1,x[i]);
	printf("\n");
	free(b);
	return x;
}
/*
Функция запроса метода Гаусса(СЛАУ или приведение матрицы к треугольному виду)
На входе
- язык интерфейса int lang(1 - английский,2 - русский)
- переменная выбора действия int j(1 - для приведения матрицы к треугольному виду,2 - для решения СЛАУ
На выходе
- код ошибки или 0, если всё ОК
Примечания
- язык по умолчанию - русский
- за правильностью значения j пользователь должен следить сам
*/
int gauss_and_SLAU_interface(int lang,int j)
{
	double **a;
	double *x;
	int n=0;
	int m=0;
	if(lang==1)
		printf("Enter the matrix without column of free members\n");
	else
		printf("Введите матрицу без столбца свободных членов\n");
	if(lang==1)
		printf("NOTE! If you want to use gauss method, enter square matrix\n");
	else
		printf("ВНИМАНИЕ! Чтобы использовать метод Гаусса, нужно ввести квадратную матрицу\n\n");
	a=matrix_input_interface(&n,&m,lang);
	if(a==NULL)
		return 1;
	if(n!=m)
	{
		if(lang==1)
			printf("Please,enter the SQUARE matrix!\n");
		else
			printf("Пожалуйста, введите КВАДРАТНУЮ матрицу!\n\n");
		getch();
		return 2;
	}
	if(lang==1)
		printf("Your matrix was:\n");
	else
		printf("Ваша матрица была:\n\n");
	output_matrix(a,n,n);
	if(j==1)
		if(gauss_matrix(a,n)!=0)
		{
			if(lang==1)
				printf("Impossible to find triangle matrix by Gauss' method.\n");
			else
				printf("Невозможно найти треугольную матрицу меиодом Гаусса.\n");
			getch();
			return 3;
		}
	if(j==2)
	{
		x=gauss_interface(a,n,lang);
		if(x==NULL)
		{
			free_matrix(a);
			return 3;
		}
	}
	if(lang==1)
		printf("The triangle matrix output:\n");
	else
		printf("Вывод треугольной матрицы:\n");
	j=matrix_output_interface(a,n,n,lang);
	if(j!=0)
	{
		free_matrix(a);
		return 4;
	}
	return 0;
}
/*
Функция интерфейса поиска суммы двух матриц
На входе:
- язык интерфейса int lang(1 - английский,2 - русский)
На выходе:
- коды ошибок или 0, если всё ОК
Примечания:
- язык по умолчанию - русский
*/
int matrix_sum_interface(int lang)
{
	double **a;
	int n=0,m=0;
	double **b;
	int k=0,l=0;
	double **c;
	if(lang==1)
		printf("The first matrix input:\n");
	else
		printf("Ввод первой матрицы:\n");
	if((a=matrix_input_interface(&n,&m,lang))==NULL)
		return 1;
	if(lang==1)
		printf("The second matrix input:\n");
	else
		printf("Ввод второй матрицы:\n");
	if((b=matrix_input_interface(&k,&l,lang))==NULL)
	{
		free_matrix(a);
		return 3;
	}
	if((c=matrix_sum(a,b,n,m,k,l))==NULL)
	{
		if(lang==1)
			printf("There was errors with sum finding\n");
		else
			printf("Возникли ошибки с нахождением суммы двух матриц\n");
		free_matrix(a);
		free_matrix(b);
		getch();
		return 4;
	}
	if(lang==1)
		printf("Your matrixes was:\nThe first:\n\n");
	else
		printf("Ваши исходные матрицы:\nПервая матрица:\n\n");
	output_matrix(a,n,m);
	if(lang==1)
		printf("The second:\n\n");
	else
		printf("Вторая матрица:\n\n");
	output_matrix(b,k,l);
	printf("\n\n");
	if((c=matrix_output_interface(c,n,m,lang))!=0)
	{
		free_matrix(a);
		free_matrix(b);
		free_matrix(c);
		return 5;
	}
	return 0;
}
int matrix_multiplication_interface(int lang)
{
	double **a;
	int n=0,m=0;
	double **b;
	int k=0,l=0;
	double **c;
	if(lang==1)
		printf("The first matrix input:\n");
	else
		printf("Ввод первой матрицы:\n");
	if((a=matrix_input_interface(&n,&m,lang))==NULL)
		return 1;
	if(lang==1)
		printf("The second matrix input:\n");
	else
		printf("Ввод второй матрицы:\n");
	if((b=matrix_input_interface(&k,&l,lang))==NULL)
	{
		free_matrix(a);
		return 3;
	}
	if((c=matrix_multiplication(a,b,n,m,k,l))==NULL)
	{
		if(lang==1)
			printf("There was some errors with matrixes multiplication. Don't forget about conforming of matrixes\n");
		else
			printf("Возникли ошибки с перемножением матриц. Не забывайте про то, что перемножать можно только конформные матрицы!\n");
		free_matrix(a);
		free_matrix(b);
		getch();
		return 4;
	}
	if(lang==1)
		printf("Your matrixes was:\nThe first:\n\n");
	else
		printf("Ваши исходные матрицы:\nПервая матрица:\n\n");
	output_matrix(a,n,m);
	if(lang==1)
		printf("The second:\n\n");
	else
		printf("Вторая матрица:\n\n");
	output_matrix(b,k,l);
	printf("\n\n");
	if(lang==1)
		printf("Result matrix output:\n");
	else
		printf("Вывод полученной матрицы:\n");
	if((c=matrix_output_interface(c,n,l,lang))!=0)
	{
		free_matrix(a);
		free_matrix(b);
		free_matrix(c);
		return 5;
	}
	return 0;
}



