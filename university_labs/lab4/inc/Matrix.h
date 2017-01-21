#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_LEN 1000

/*
������� ������ ������� ���� double � �������� ����.
�� ����� 
- ��� �����,
��������� �� �������,
����������� ������� - n � m
�� ������ - ��� ������ ��� 0, ���� �� ��.
1 - ������ �������� �����.
2 - ������ ������ ����������� � ����.
3 - ������ ������ �������.
*/
int write_bin_matrix(char *fname, double **a,int n,int m);


/*
������� ������ ������� ���� double �� ��������� �����.
�� ����� - ��� �����, ��������� �� ���������� ���� int,
�� ������ ������� ����� �������� ����������� ������.
�� ������ - ��������� �� ������� ��� NULL � ������ ������.
������ ������������ ������ ���������� ���.
*/
double **read_bin_matrix(char *fname, int *n, int *m);


/*
������� ������ ������� ���� double � ��������� ����.
�� ����� - ��� �����, ��������� �� ������, ����������� n � m.
�� ������ - 
0 - � ������, ���� �� ��.
1 - � ������, ���� �� ������� ������� ����.
*/
int write_txt_matrix(char *fname,double **a,int n,int m);



/*
������� ������ ������� �� ���������� �����.
�� ����� - ��� �����, ��������� �� ����������� �������.
�� ������ - ��������� �� ������� ��� NULL � ������ ������.
*/
double **read_txt_matrix(char *fname, int *n, int *m);



/*
������� ����� ������� �� ������������ ���������� �����.
�� �����
- ����������� ������� int n � int m
- ���� ���������� int lang(1 - ��� �����������,2 - ��� ��������)
�� ������ 
- ��������� �� ������� ��� NULL � ������ ������.
����������:
- ������ ������������� ������ ���������� ���
- ���� �� ��������� - �������
*/
double **input_matrix(int n, int m,int lang);


void output_matrix(double **a,int n, int m);


/*
������� ���������� ������� ���� float ���������� �������.
�� ����� - �����������.
�� ������ - ��������� �� ������� ��� NULL � ������ ������.
*/
double **rnd_matrix(int n, int m);

/*
������� �������� ���� ������.
�� ����� - ��������� �� ������� � ����������� n, m, k, l.
�� ������ - ��������� �� ������� ��� NULL � ������ ������.
*/
double **matrix_sum(double **a, double **b, int n, int m, int k, int l);


/*
������� ������������ ���� ������.
�� ����� - ��������� �� ��� �������, �� ����������� n, m, k, l.
�� ������ - ��������� �� �������-��������� ����������� n*l ��� NULL � ������ ������.
*/
double **matrix_multiplication(double **a, double **b, int n, int m, int k, int l);

double **malloc_matrix(int n,int m);

void free_matrix(int **pa);

int gauss_matrix(double **a, int n);

/*
������� ������� ����� �������.
�� �����:
- ������� ���� double **a
- ���� ���������� int lang(1 - ����������,2 - �������)
�� ������:
- ������� ���� double **a ��� NULL � ������ ������
- ����������� n � m � ������� ����
����������:
- ������ ������������ ������ ����������� ��� �������� free_matrix
- ���� �� ��������� - �������
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
