#include <iostream>
#include "32.h"
using namespace std;

int main()
{
	int num;
	do
	{
		cout <<
			"Введите числа (-1 для выхода, -2 для сброса): ";
		cin >> num;
		if (num == -2) { reset(); continue; }
		if (num != -1)
			cout << "Среднее значение равно: " << r_avg(num);
		cout << '\n';
	} while (num != -1);

	return 0;
}
