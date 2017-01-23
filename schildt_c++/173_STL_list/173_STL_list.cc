#include <iostream>
#include <list>
using namespace std;

int main()
{
    list<char> lst;
    int i;

    for(i=0; i<10; i++) lst.push_back('A' + i);
    cout << "Размер = " << lst.size() << endl;
    cout << "Содержимое : ";
    list<char>::iterator p = lst.begin();
    while(p != lst.end())
    {
        cout << *p;
        p++;
    }

    return 0;
}
