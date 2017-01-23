#include <iostream>
#include <map>
#include <cstring>
using namespace std;

class word
{
    char str[20];
public:
    word() { strcpy(str, ""); }
    word(char *s) { strcpy(str, s); }
    char *get() { return str; }
};

bool operator<(word a, word b)
{
    return strcmp(a.get(), b.get()) < 0;
}

class meaning
{
    char str[80];
public:
    meaning() { strcpy(str, ""); }
    meaning(char *s) { strcpy(str, s); }
    char *get() { return str; }
};

int main()
{
    map<word, meaning> dictionary;

    dictionary.insert(pair<word, meaning>(
                           word("дом"),
                           meaning("Место проживания.")));
    dictionary.insert(pair<word, meaning>(
                           word("клавиатура"),
                           meaning("Устройство ввода данных.")));
    dictionary.insert(pair<word,meaning>(
                      word("программирование"),
                      meaning("Процесс создания программы.")));
    dictionary.insert(pair<word, meaning>(
                      word("STL"),
                      meaning("Standart Template Library")));

    char str[80];
    cout << "Введите слово: ";
    cin >> str;

    map<word, meaning>::iterator p;

    p = dictionary.find(word(str));
    if(p != dictionary.end())
        cout << "Определение: " << p->second.get();
    else
        cout << "Такого слова в словаре нет.\n";

    return 0;
}
