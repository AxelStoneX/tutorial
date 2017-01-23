#include <iostream>
using namespace std;

struct mystruct
{
	int a;
	int b;
};

mystruct &f(mystruct &var);

int main()
{
	mystruct x, y;
	x.a = 10; x.b = 20;

	cout << "Исходные значения полей x.a и x.b: ";
	cout << x.a << ' ' << x.b << '\n';

	y = f(x);
	cout << "Модифицированные значения полей x.a и x.b: ";
	cout << x.a << ' ' << x.b << '\n';
	cout << "Модифицированные значения полей y.a и y.b: ";
	cout << y.a << ' ' << y.b << '\n';

	return 0;
}

mystruct &f(mystruct &var)
{
	var.a = var.a * var.a;
	var.b = var.b / var.b;
	return var;
}
