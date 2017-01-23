#include <iostream>
using namespace std;

void Xhandler()
{
    try
    {
        throw "Привет";
    }
    catch (char *)
    {
        cout << "Перехват исключения в функции Xhandler.\n";
        throw;
    }
}

int main()
{
    cout << "НАЧАЛО\n";

    try
    {
        Xhandler();
    }
    catch(char *)
    {
        cout << "Перехват исключения в функции main().\n";
    }

    cout << "КОНЕЦ";

    return 0;
}
