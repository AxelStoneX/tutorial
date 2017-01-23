#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

int xform(int i)
{
    return i * i;
}

int main()
{
    list<int> x1;
    int i;

    for(i=0; i<10; i++) x1.push_back(i);

    cout << "Исходный список x1: ";
    list<int>::iterator p = x1.begin();
    while(p != x1.end())
    {
        cout << *p << " ";
        p++;
    }

    cout << endl;

    p = transform(x1.begin(), x1.end(), x1.begin(), xform);

    cout << "Преобразованный список x1: ";
    p = x1.begin();
    while(p != x1.end())
    {
        cout << *p << " ";
        p++;
    }

    return 0;
}
