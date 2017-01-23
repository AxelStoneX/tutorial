#include <iostream>
using namespace std;

class myclass
{
	int a, b;
public:
	void setab(int i, int j) { a = i, b = j; }
	void showab();
};

void myclass::showab()
{
	cout << "a равно " << a << '\n';
	cout << "b равно " << b << '\n';
}

int main()
{
	myclass ob1, ob2;
	ob1.setab(10, 20);
	ob2.setab(0, 0);
	cout << "Объект ob1 до присваивания: \n";
	ob1.showab();
	cout << "Объект ob2 до присваивания: \n";
	ob2.showab();

	ob2 = ob1;
	cout << "Объект ob1 после присваивания: \n";
	ob1.showab();
	cout << "Объект ob2 после присваивания: \n";
	ob2.showab();

	return 0;
}
