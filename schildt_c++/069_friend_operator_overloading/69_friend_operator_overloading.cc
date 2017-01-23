#include <iostream>
using namespace std;

class CL
{
public:
	int count;
	CL operator=(CL obj);
	friend CL operator+(CL ob, int i);
	friend CL operator+(int i, CL ob);
};

CL CL::operator=(CL obj)
{
	count = obj.count;
	return *this;
}

CL operator+(CL ob, int i)
{
	CL temp;

	temp.count = ob.count + i;
	return temp;
}

CL operator+(int i, CL ob)
{
	CL temp;

	temp.count = ob.count + i;
	return temp;
}

int main()
{
	CL O;

	O.count = 10;
	cout << O.count << " ";

	O = 10 + O;
	cout << O.count << " ";

	O = O + 12;
	cout << O.count << " ";

	return 0;
}
