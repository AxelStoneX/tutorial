#include <iostream>
using namespace std;

class myclass
{
public:
	myclass() { cout << "Это обычный конструктор.\n"; }
	myclass(const myclass &obj)
	{ cout << "Конструктор копии.\n"; }
};

myclass f()
{
	myclass ob;
	return ob;
}

int main()
{
	myclass a;

	a = f();

	return 0;
}
