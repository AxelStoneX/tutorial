#include <iostream>
using namespace std;

class base1
{
protected:
	int i;
public:
	base1(int x) { i = x;
	               cout << "Создание base1-объекта.\n"; }
	~base1() { cout << "Разрушение base1 объекта.\n"; }
};

class base2
{
protected:
	int k;
public:
	base2(int x) { k = x;
	               cout << "Создание base2-объекта.\n"; }
	~base2() { cout << "Разрушение base2-объекта.\n"; }
};

class derived: public base1, public base2
{
public:
	derived(int x, int y): base1(x), base2(y)
	{ cout << "Создание derived-объекта.\n"; }
	~derived() { cout << "Разрушение derived-объекта.\n"; }
	void show() { cout << i << " " << k << "\n"; }
};

int main()
{
	derived ob(3, 4);
	ob.show();
	return 0;
}
