#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    char str[80];

    cout << "Введите имя: ";
    cin.get(str, 79);

    cout << str << '\n';
    return 0;
}
