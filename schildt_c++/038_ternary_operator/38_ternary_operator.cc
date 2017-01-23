#include <iostream>
using namespace std;

int div_zero();

int main()
{
	int i, j, result;

	cout << "Введите делимое и делитель: \n";
	cin >> i >> j;

	result = j ? i/j : div_zero();
	cout << "Результат: " << result;

	return 0;
}

int div_zero()
{
	cout << "Нельзя делить на нуль.\n";
	return 0;
}
