#include <iostream>
using namespace std;

void Xhandler(int test)
{
    try
    {
        if(test) throw test;
        else throw "Значение равно нулю.";
    }
    catch(int i)
    {
        cout << "Перехват! Исключение №: " << i << '\n';
    }
    catch(char *str)
    {
        cout << "Перехват строки: ";
        cout << str << '\n';
    }
}

int main()
{
    cout << "НАЧАЛО\n";

    Xhandler(1);
    Xhandler(2);
    Xhandler(3);
    Xhandler(0);

    cout << "КОНЕЦ";

    return 0;
}
