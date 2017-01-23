#include <iostream>
using namespace std;

class base
{
	int i;
public:
	int j, k;
	void seti(int x) { i = x; }
	int geti() { return i; }
};

class derived: private base
{
public:
	base::j;
	base::seti;
	base::geti;

	int a;
};

int main()
{
	derived ob;
	ob.j = 20;
	ob.a = 40;
	ob.seti(10);
	cout << ob.geti() << " " << ob.j << " " << ob.a;
	return 0;
}
