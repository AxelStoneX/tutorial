#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class sample
{
	char *s;
public:
	sample();
	sample(const sample &ob);
	~sample() { if(s) delete [] s;
	            cout << "Освобождение памяти.\n"; }
	void show() { cout << s << '\n'; }
	void set(char *str);
	sample operator=(sample &ob);
};

sample::sample()
{
	s = new char('\0');
}

sample::sample(const sample &ob)
{
	s = new char[strlen(ob.s)+1];
	strcpy(s, ob.s);
}

void sample::set(char *str)
{
	s = new char[strlen(str) + 1];
	strcpy(s, str);
}

sample sample::operator=(sample &ob)
{
	if(strlen(ob.s) > strlen(s))
	{
		delete [] s;
		s = new char[strlen(ob.s) + 1];
		strcpy(s, ob.s);
		return *this;
	}
}

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
