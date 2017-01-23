#include <iostream>
using namespace std;

void Xhandler(int test)
{
    try
    {
        if(test==0) throw test;
        if(test==1) throw 'a';
        if(test==2) throw 123.23;
    }
    catch(...)
    {
        cout << "Перехват!\n";
    }
}

int main()
{
    cout << "НАЧАЛО\n";

    Xhandler(0);
    Xhandler(1);
    Xhandler(2);

    cout << "КОНЕЦ";

    return 0;
}
