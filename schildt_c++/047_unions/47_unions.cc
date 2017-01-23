#include <iostream>
//#include <conio.h>
using namespace std;

struct byte
{
	unsigned a : 1;
	unsigned b : 1;
	unsigned c : 1;
	unsigned d : 1;
	unsigned e : 1;
	unsigned f : 1;
	unsigned g : 1;
	unsigned h : 1;
};

union bits
{
	char ch;
	struct byte bit;
} ascii;

void disp_bits(bits b);

int main()
{
	do
	{
		cin >> ascii.ch;
		cout << ":";
		disp_bits(ascii);
	} while(ascii.ch != 'q');

	return 0;
}

void disp_bits(bits b)
{
	if(b.bit.h) cout << "1 ";
	else cout << "0 ";
	if(b.bit.g) cout << "1 ";
	else cout << "0 ";
	if(b.bit.f) cout << "1 ";
	else cout << "0 ";
	if(b.bit.e) cout << "1 ";
	else cout << "0 ";
	if(b.bit.d) cout << "1 ";
	else cout << "0 ";
	if(b.bit.c) cout << "1 ";
	else cout << "0 ";
	if(b.bit.b) cout << "1 ";
	else cout << "0 ";
	if(b.bit.a) cout << "1 ";
	else cout << "0 ";

	cout << "\n";
}
