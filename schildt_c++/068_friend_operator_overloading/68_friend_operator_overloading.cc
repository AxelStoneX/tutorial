#include <iostream>
using namespace std;

class three_d
{
	int x, y, z;
public:
	three_d() { x = y = z = 0; }
	three_d(int i, int j, int k ) { x = i; y = j; z = k; }
	friend three_d operator+(three_d op1, three_d op2);
	three_d operator=(three_d op2);

	void show();
};

three_d operator+(three_d op1, three_d op2)
{
	three_d temp;
	temp.x = op1.x + op2.x;
	temp.y = op1.y + op2.y;
	temp.z = op1.z + op2.z;
	return temp;
}

three_d three_d::operator=(three_d op2)
{
	x = op2.x;
	y = op2.y;
	z = op2.z;
	return *this;
}

void three_d::show()
{
	cout << x << ", ";
	cout << y << ", ";
	cout << z << "\n";
}

int main()
{
	three_d a(1, 2, 3), b(10, 10, 10), c;
	a.show();
	b.show();
	cout << "\n";
	c = a + b;
	c.show();

	c = a + b;
	c.show();

	c = a + b + c;
	c.show();

	c = b = a;
	c.show();
	b.show();

	return 0;
}
