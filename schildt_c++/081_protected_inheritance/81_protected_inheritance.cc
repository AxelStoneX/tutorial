#include <iostream>
using namespace std;

class base
{
	int i;
protected:
	int j;
public:
	int k;
	void seti(int a) { i = a; }
	int geti() { return i; }
};

class derived : protected base
{
public:
	void setj(int a) { j = a; }
	void setk(int a) { k = a; }
	int getj() { return j; }
	int getk() { return k; }
};

int main()
{
	derived ob;
	ob.setk(10);
	cout << ob.getk() << ' ';
	ob.setj(12);
	cout << ob.getj() << ' ';

	return 0;
}
