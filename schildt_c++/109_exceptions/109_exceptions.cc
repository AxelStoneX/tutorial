#include <iostream>
using namespace std;

void Xhandler(int test)
{
    try
    {
        if (test) throw test;
    }
    catch (int i)
    {
        cout << "Перехват! Исключение №: " << i << '\n';
    }
}

int main()
{
    cout << "НАЧАЛО\n";

    Xhandler(1);
    Xhandler(2);
    Xhandler(0);
    Xhandler(3);

    cout << "КОНЕЦ";

    return 0;
}
