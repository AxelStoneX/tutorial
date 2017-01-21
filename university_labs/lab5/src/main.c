#include "Matrix.h"
int main(void)
{
	int j;
	int lang;
	setlocale( LC_CTYPE, ".1251" );
	printf("-------------Matrix Calculator-------------\n");
	printf("-------------version 1.2.1.2.1-------------\n");
	printf("--------Written by Alexander Tzukanov------\n\n");
	printf("What language do you want to use\n(Выберите язык программы)\n1 - for english\n2 - для русского языка\n");
	scanf("%d",&lang);
	printf("\n");
	if(lang!=1 && lang!=2)
	{
		printf("You pressed wrong key\n(Вы нажали не ту кнопку)\n");
		getch();
		return 5;
	}
	if(lang==1)
		printf("What do you want to do today?\n1 - for triangle matrix getting\n2 - for SLAU solving\n3 - for matrixes sum finding\n4 - for matrixes multiplication\n");
	else
		printf("Что вы изволите сделать сегодня?\n1 - для приведения матрицы к треугольному виду\n2 - для решения СЛАУ\n3 - для суммирования двух матриц\n4 - для перемножения двух матриц\n");
	scanf("%d",&j);
	printf("\n");
	if(j!=1 && j!=2 && j!=3 && j!= 4)
	{
		if(lang==1)
			printf("You pressed wrong key\n");
		else
			printf("Вы нажали не ту кнопку\n");
		getch();
		return 5;
	}
	if(j==1 || j==2)
		if(gauss_and_SLAU_interface(lang,j)!=0)
			return 6;
	if(j==3)
		if(matrix_sum_interface(lang)!=0)
			return 7;
	if(j==4)
		if(matrix_multiplication_interface(lang)!=0)
			return 8;
	if(lang==1)
		printf("Success!\nPress any key to continue\n");
	else
		printf("Успех!\nНажмите любую кнопку для выхода из приложения\n");
	getch();
	return 0;
}













