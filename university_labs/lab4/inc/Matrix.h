#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_LEN 1000

/*
Функция записи матрицы типа double в бинарный файл.
На входе 
- имя файла,
указатель на матрицу,
размерность матрицы - n и m
На выходе - код ошибки или 0, если всё ОК.
1 - ошибка открытия файла.
2 - ошибка записи размерности в файл.
3 - ошибка записи массива.
*/
int write_bin_matrix(char *fname, double **a,int n,int m);


/*
Функция чтения матрицы типа double из бинарного файла.
На входе - имя файла, указатели на переменные типа int,
по адресу которых будет записана размерность матриц.
На выходе - указатель на матрицу или NULL в случае ошибок.
Паиять пользователь должен освободить сам.
*/
double **read_bin_matrix(char *fname, int *n, int *m);


/*
Функция записи массива типа double в текстовый файл.
На входе - имя файла, указатель на массив, размерность n и m.
На выходе - 
0 - в случае, если всё ОК.
1 - в случае, если не удалось открыть файл.
*/
int write_txt_matrix(char *fname,double **a,int n,int m);



/*
Функция чтения матрицы из текстового файла.
На входе - имя файла, указатель на размерности матрицы.
На выходе - указатель на матрицу или NULL в случае ошибок.
*/
double **read_txt_matrix(char *fname, int *n, int *m);



/*
Функция ввода массива со стандартного устройства ввода.
На входе
- размерность матрицы int n и int m
- язык интерфейса int lang(1 - для английского,2 - для русского)
На выходе 
- указатель на матрицу или NULL в случае ошибки.
Примечания:
- память пользователшь должен освободить сам
- язык по умолчанию - русский
*/
double **input_matrix(int n, int m,int lang);


void output_matrix(double **a,int n, int m);


/*
Функция заполнения матрицы типа float случайными числами.
На входе - размерность.
На выходе - указатель на матрицу или NULL в случае ошибок.
*/
double **rnd_matrix(int n, int m);

/*
Функция сложения двух матриц.
На входе - указатели на матрицы и размерности n, m, k, l.
На выходе - указатель на матрицу или NULL в случае ошибки.
*/
double **matrix_sum(double **a, double **b, int n, int m, int k, int l);


/*
Функция перемножения двух матриц.
На входе - указатели на две матрицы, их размерности n, m, k, l.
На выходе - указатель на матрицу-результат размерности n*l или NULL в случае ошибки.
*/
double **matrix_multiplication(double **a, double **b, int n, int m, int k, int l);

double **malloc_matrix(int n,int m);

void free_matrix(int **pa);

int gauss_matrix(double **a, int n);

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
double **matrix_input_interface(int *n, int *m, int lang);

int matrix_output_interface(double **a,int n,int m, int lang);

int row_swap(double **a, int k, int f);

double *gauss_interface(double **a,int n, int lang);

int gauss_slau(double **a,double *b,int n);

double *gauss_slau_reverse(double **a,double *b,int n);

void swap_freerow(double *b,int k,int f);

int gauss_and_SLAU_interface(int lang,int j);

int matrix_sum_interface(int lang);

int matrix_multiplication_interface(int lang);
