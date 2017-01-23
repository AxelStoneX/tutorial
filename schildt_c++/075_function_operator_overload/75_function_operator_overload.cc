#include <iostream>
using namespace std;

class three_d
{
	int x, y, z;
public:
	three_d() { x = y = z = 0; }
	three_d(int i, int j, int k) { x = i; y = j; z = k; }
	three_d operator() (int a, int b, int c);
	void show();
};

three_d three_d::operator() (int a, int b, int c)
{
	three_d temp;

	temp.x = x + a;
	temp.y = y + b;
	temp.z = z + c;

	return temp;
}

void three_d::show()
{
	cout << x << ", ";
	cout << y << ", ";
	cout << z << "\n";
}

int main()
{
	three_d ob1(1, 2, 3), ob2;
	ob2 = ob1(10, 11, 12);

	cout << "ob1: ";
	ob1.show();

	cout << "ob2: ";
	ob2.show();

	return 0;
}
