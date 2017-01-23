#include <iostream>
#include <map>
using namespace std;

int main()
{
    map<char, int> m;
    int i;

    for(i=0; i<10; i++)
    {
        m.insert(pair<char, int>('A'+i, i));
    }

    char ch = 'A';
    cout << "Введите ключ: ";
    cin >> ch;

    map<char, int>::iterator p;

    p=m.find(ch);
    if(p != m.end())
        cout << p->second;
    else
        cout << "Такого ключа в отображении нет.\n";

    return 0;
}
