#include <iostream>
#include "29.h"
using namespace std;

int x,y;
char ch;

int main()
{
	x = 100;
	y = 10;
	cout << "x = " << x << " y = " << y << "\n";
	func1();
	cout << "x = " << x << " y = " << y << "\n";
	func22();
	cout << "x = " << x << " y = " << y << "\n";
	func23();

	return 0;
}

void func1()
{
	x = 123;
}
