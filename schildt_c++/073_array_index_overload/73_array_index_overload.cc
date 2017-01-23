#include <iostream>
using namespace std;

const int SIZE = 3;

class atype
{
	int a[SIZE];
public:
	atype()
	{
		register int i;
		for(i=0; i<SIZE; i++) a[i] = i;
	}
	int &operator[](int i) { return a[i]; }
};

int main()
{
	atype ob;

	cout << ob[2];
	cout << " ";
	ob[2] = 25;
	cout << ob[2];
	return 0;
}
