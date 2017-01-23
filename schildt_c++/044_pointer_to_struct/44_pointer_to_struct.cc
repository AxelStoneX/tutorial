#include <iostream>
#include <ctime>
using namespace std;

int main()
{
	struct tm *ptr;
	time_t lt;

	lt = time('\0');

	ptr = localtime(&lt);
	cout << asctime(ptr);

	return 0;
}
