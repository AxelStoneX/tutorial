#include <iostream>
using namespace std;

class myclass
{
	int val;
public:
	myclass(int i) { val = i; cout << "Объект создан\n"; }
	~myclass() { cout << "Объект разрушен\n"; }
	int getval() { return val; }
};

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
