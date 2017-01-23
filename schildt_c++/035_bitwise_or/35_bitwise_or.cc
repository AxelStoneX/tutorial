#include <iostream>
using namespace std;

int main()
{
	char ch = 'a';

	do
	{
		cin >> ch;
		ch = ch | 32;
		cout << ch << '\n';
	} while (ch != 'q');

	return 0;
}
