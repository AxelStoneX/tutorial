#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
    map<string, string> dictionary;

    dictionary.insert(pair<string, string>(
                             "дом", "Место проживания."));
    dictionary.insert(pair<string, string>(
               "клавиатура", "Устройство ввода данных."));
    dictionary.insert(pair<string, string>(
            "программирование", "Процесс создания программы"));
    dictionary.insert(pair<string, string>(
            "STL", "Standart Template Library"));

    string s;
    cout << "Введите слово: ";
    cin >> s;

    map<string, string>::iterator p;

    p = dictionary.find(s);
    if(p != dictionary.end())
        cout << "Определение: " << p->second;
    else
        cout << "Такого слова в словаре нет.\n";

    return 0;
}
