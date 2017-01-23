#include <iostream>
using namespace std;

void Xtest(int test)
{
    cout << "В теле функции Xtest(), значение test равно: "
         << test << "\n";
    if (test) throw test;
}

int main()
{
    cout << "НАЧАЛО\n";

    try
    {
        cout << "В try-блоке.\n";
        Xtest(0);
        Xtest(1);
        Xtest(2);
    }
    catch(int i)
    {
        cout << "Перехват исключения. Его значение равно: ";
        cout << i << "\n";
    }

    cout << "КОНЕЦ.\n";

    return 0;
}
