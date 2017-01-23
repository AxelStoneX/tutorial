#include <iostream>
using namespace std;

class three_d
{
	int x, y, z;
public:
	three_d() { x = y = z = 0; }
	three_d(int i, int j, int k) { x = i; y = j; z = k; }

	friend three_d operator+(three_d op1, three_d op2);
	three_d operator=(three_d op2);

	friend three_d operator++(three_d &op1);
	friend three_d operator++(three_d &op1, int notused);

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

three_d operator++(three_d &op1)
{
	op1.x++;
	op1.y++;
	op1.z++;
	return op1;
}

three_d operator++(three_d &op1, int notused)
{
	three_d temp = op1;
	op1.x++;
	op1.y++;
	op1.z++;
	return temp;
}

void three_d::show(){
	cout << x << ", ";
	cout << y << ", ";
	cout << z << "\n";
}

int main()
{
	three_d a(1, 2, 3), b(10, 10, 10), c;

	a.show();
	b.show();

	c = a + b;
	c.show();

	c = a + b + c;
	c.show();

	c = b = a;
	c.show();
	b.show();

	++c;
	c.show();

	c++;
	c.show();

	a = ++c;
	a.show();
	c.show();

	a = c++;

	a.show();
	c.show();

	return 0;
}
