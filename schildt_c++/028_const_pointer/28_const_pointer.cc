#include <iostream>
using namespace std;

void code(const char *str);

int main(void)
{
	code("This is test");
	return 0;
}

void code(const char *str)
{
	while(*str)
	{
		cout << (char) (*str+1);
		str++;
	}
}
