#include <iostream>
using namespace std;

void disp_binary(unsigned u);

int main(void)
{
	disp_binary(0xFFFF);
	disp_binary(0xFFFE);

	return 0;
}

void disp_binary(unsigned u)
{
	register int t;

	for(t=128; t>0; t = t/2)
		if(u & t) cout << "1 ";
		else      cout << "0 ";
	cout << "\n";
}
