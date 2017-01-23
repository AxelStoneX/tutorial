#include <iostream>
#include <cstdlib>
using namespace std;

class myclass
{
	int *p;
public:
	myclass(int i);
	myclass(const myclass &obj);
	~myclass();
	int getval() { return *p; }
};

myclass::myclass(const myclass &obj)
{
	p = new int;
	*p = *obj.p;
	cout << "Вызван конструктор копии.\n";
}

myclass::myclass(int i)
{
	cout << "Выделение памяти, адресуемой указателем p.\n";
	p = new int;
	*p = i;
}

myclass::~myclass()
{
	cout << "Освобождение памяти, адресуемой указателем p.\n";
	delete p;
}

void display(myclass ob)
{
	cout << ob.getval() << '\n';
}

int main()
{
	myclass a(10);

	display(a);

	return 0;
}
