#include <iostream>
#include <list>
using namespace std;

int main()
{
    list<char> lst;
    list<char> revlst;
    int i;

    for(i=0; i<10; i++) lst.push_back('A' + i);
    cout << "Размер списка lst: " << lst.size() << endl;
    cout << "Исходное содержимое списка: ";

    list<char>::iterator p;

    while(!lst.empty())
    {
        p = lst.begin();
        cout << *p;
        revlst.push_front(*p);
        lst.pop_front();
    }
    cout << endl << endl;

    cout << "Размер списка revlst = ";
    cout << revlst.size() << endl;

    cout << "Реверсированное содержимое списка: ";
    p = revlst.begin();
    while(p != revlst.end())
    {
        cout << *p;
        p++;
    }

    return 0;
}
