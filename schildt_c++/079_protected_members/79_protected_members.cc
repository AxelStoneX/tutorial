#include <iostream>
using namespace std;

class base
{
protected:
	int i, j;
public:
	void set(int a, int b) { i = a; j = b; }
	void show() { cout << i << " " << j << "\n"; }
};

class derived : public base
{
	int k;
public:
	void setk() { k = i*j; }
	void showk() { cout << k << "\n"; }
};

int main()
{
	derived ob;

	ob.set(2, 3);
	ob.show();

	ob.setk();
	ob.showk();

	return 0;
}
