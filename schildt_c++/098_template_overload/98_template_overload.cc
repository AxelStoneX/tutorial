#include <iostream>
using namespace std;

template <class X> void f (X a)
{
    cout << "Выполняется функция f(X a)\n";
}

template <class X, class Y> void f (X a, Y b)
{
    cout << "Выполняется функция f(X a, Y b)\n";
}

int main()
{
    f(10);
    f(10,20);

    return 0;
}
