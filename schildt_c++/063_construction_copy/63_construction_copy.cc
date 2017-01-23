#include <iostream>
#include <cstdlib>
using namespace std;

class myclass
{
	int *p;
public:
	myclass(int i);
	myclass(const myclass &ob);
	~myclass();
	int getval() { return *p; }
};

myclass::myclass(const myclass &ob)
{
	p = new int;
	*p = *ob.p;
	cout << "Выделение p-памяти конструктором копии.\n";
}

myclass::myclass(int i)
{
	cout << "Выделение p-памяти обычным конструктором.\n";
	p = new int;
	*p = i;
}

myclass::~myclass()
{
	cout << "Освобождение p-памяти.\n";
	delete p;
}

int main(void)
{
	myclass a(10);

	myclass b = a;

	return 0;
}
