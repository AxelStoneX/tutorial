#include <iostream>
using namespace std;

int main()
{

	union
	{
		short int count;
		char ch[2];
	};

	ch[0] = 'X';
	ch[1] = 'Y';
	cout << "Объединение в виде символов: " << ch[0]
			                                << ch[1] << '\n';
	cout << "Объединения в виде целого значения: " <<
			                                 count << '\n';

	return 0;
}
