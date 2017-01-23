#include <iostream>
using namespace std;

class base
{
protected:
	int i;
public:
	base(int x)
	{
		i = x;
		cout << "Создание base-объекта.\n";
	}
	~base() { cout << "Разрушение base1-объекта.\n"; }
};

class derived: public base
{
	int j;
public:
	derived(int x, int y) : base(y)
	{   j = x; cout << "Создание derived-объекта.\n"; }
	~derived() { cout << "Разрушение derived-объекта.\n"; }
	void show() { cout << i << " " << j << "\n"; }
};

int main()
{
	derived ob(3, 4);
	ob.show();
	return 0;
}
