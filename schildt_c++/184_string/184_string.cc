#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str1("Класс string позволяет эффективно ");
    string str2("обрабатывать строки.");
    string str3;

    str3 = str1;
    cout << str1 << "\n" << str3 << "\n";

    str3 = str1 + str2;
    cout << str3 << "\n";

    if(str3 > str1) cout << "str3 > str1\n";
    if(str3 == str1+str2)
        cout << "str3 == str1+str2\n";

    str1 = "Это строка с завершающим нулем.\n";
    cout << str1;

    string str4(str1);
    cout << str4;

    cout << "Введите строку: ";
    cin >> str4;
    cout << str4;

    return 0;
}
