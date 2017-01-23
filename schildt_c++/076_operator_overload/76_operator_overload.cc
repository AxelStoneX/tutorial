#include <iostream>
#include <cstring>
using namespace std;

class str_type
{
	char string[80];
public:
	str_type(char *str = "") { strcpy(string, str); }

	str_type operator+(str_type str);
	str_type operator+(char *str);

	str_type operator=(str_type str);
	str_type operator=(char *str);

	void show_str() { cout << string; }
};

str_type str_type::operator+(str_type str)
{
	str_type temp;

	strcpy(temp.string, string);
	strcat(temp.string, str.string);
	return temp;
}

str_type str_type::operator=(str_type str)
{
	strcpy(string, str.string);
	return *this;
}

str_type str_type::operator=(char *str)
{
	str_type temp;

	strcpy(string, str);
	strcpy(temp.string, string);
	return temp;
}

str_type str_type::operator+(char *str)
{
	str_type temp;
	strcpy(temp.string, string);
	strcat(temp.string, str);
	return temp;
}

int main()
{
	str_type a("Привет "), b("всем"), c;

	c = a + b;

	c.show_str();
	cout << "\n";

	a = "для программирования, потому что";
	a.show_str();
	cout << "\n";

	b = c = "С++ это супер";

	//c = c + " " + a + " " + b;
	c.show_str();

	return 0;
}
