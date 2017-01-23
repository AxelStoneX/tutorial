#include <iostream>
using namespace std;

extern "C" void myCfunc();

int main()
{
    myCfunc();
    return 0;
}

void myCfunc()
{
    cout << "Эта функция скомпонована как C функция.\n";
}
