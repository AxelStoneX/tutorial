#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str1("Это простой тест.");
    string str2("ABCDEFG");

    cout << "Исходные строки:\n";
    cout << "str1: " << str1 << endl;
    cout << "str2: " << str2 << "\n\n";

    cout << "Вставляем строку str2 в строку str1:\n";
    str1.insert(5, str2);
    cout << str1 << "\n\n";

    cout << "Удаляем 7 символов из строки str1:\n";
    str1.erase(5, 7);
    cout << str1 << "\n\n";

    cout << "Заменяем 2 символа в str1 строкой str2:\n";
    str1.replace(5, 2, str2);
    cout << str1 << endl;

    return 0;
}
