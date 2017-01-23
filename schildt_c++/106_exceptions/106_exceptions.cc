#include <iostream>
using namespace std;

int main()
{
    cout << "Начало\n";

    try
    {
        cout << "В try блоке.\n";
        throw 99;
        cout << "Эта инструкция не будет выполнена";
    }
    catch(int i)
    {
        cout << "Перехват исключения. Его значение равно: ";
        cout << i << "\n";
    }

    cout << "Конец.\n";

    return 0;
}
