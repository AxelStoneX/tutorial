#include <iostream>
#include <iomanip>
using namespace std;

istream &prompt(istream &stream)
{
    cout << "Введите число в шестнадцатеричном формате: ";
    cin >> hex;

    return stream;
}

int main()
{
    int i;

    cin >> prompt >> i;
    cout << i;

    return 0;
}
