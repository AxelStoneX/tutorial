#include <iostream>
using namespace std;

void Xhandler(int test) throw(int, char, double)
{
    if(test==0) throw test;
    if(test==1) throw 'a';
    if(test==2) throw 123.23;
}

int main()
{
    cout << "НАЧАЛО\n";
    try
    {
        Xhandler(2);
    }
    catch(int i)
    {
        cout << "Перехват int-исключения.\n";
    }
    catch(char c)
    {
        cout << "Перехват char-исключения.\n";
    }
    catch(double d)
    {
        cout << "Перехват double-исключения.\n";
    }

    cout << "КОНЕЦ";

    return 0;
}
