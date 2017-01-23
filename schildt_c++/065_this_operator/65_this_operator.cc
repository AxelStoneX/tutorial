#include <iostream>
using namespace std;

class c1
{
	int i;
public:
	void load_i(int val)
	{ this->i = val; }
	int get_i()
	{ return this->i; }
};

int main()
{
	c1 o;
	o.load_i(100);
	cout << o.get_i();

	return 0;
}
