#include <iostream>
using namespace std;

void disp_binary(unsigned u);

int main()
{
	unsigned u;

	cout << "Введите число между 0 и 255: ";
	cin >> u;

	cout << "Исходное число в двоичном коде: ";
	disp_binary(u);

	cout << "Его дополнение до единицы: ";
	disp_binary(~u);

	return 0;
}

void disp_binary(unsigned u)
{
	register int t;

	for(t=128; t>0; t = t/2)
		if(u & t) cout << "1 ";
		else      cout << "0 ";
	cout << '\n';
}
