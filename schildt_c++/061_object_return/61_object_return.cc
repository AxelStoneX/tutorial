#include <iostream>
#include <cstring>
using namespace std;

class sample
{
	char s[80];
public:
	void show() { cout << s << "\n"; }
	void set(char *str) { strcpy(s, str); }
};

sample input()
{
	char instr[80];
	sample str;

	cout << "Введите строку: ";
	cin >> instr;

	str.set(instr);

	return str;

}

int main()
{
	sample ob;

	ob = input();
	ob.show();

	return 0;
}
