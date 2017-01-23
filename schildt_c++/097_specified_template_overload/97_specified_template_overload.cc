#include <iostream>
using namespace std;

template <class X> void swapargs(X &a, X &b)
{
    X temp;

    temp = a;
    a = b;
    b = temp;
    cout << "Выполняется шаблонная функция swapargs.\n";
}

void swapargs(int &a, int &b)
{
    int temp;

    temp = a;
    a = b;
    b = temp;
    cout << "Выполняется int-специализация функции swapargs.\n";
}

int main()
{
    int i=10, j=20;
    double x=10.1, y=23.3;
    char a='x', b='z';

    cout << "Исходные значения i, j: " << i << ' '
                                       << j << '\n';
    cout << "Исходные значения x, y: " << x << ' '
                                       << y << '\n';
    cout << "Исходные значения a, b: " << a << ' '
                                       << b << '\n';

    swapargs(i, j);
    swapargs(x, y);
    swapargs(a, b);

    cout << "После перестановки i, j: " << i << ' '
                                        << j << '\n';
    cout << "После перестановки x, y: " << x << ' '
                                        << y << '\n';
    cout << "После перестановки a, b: " << a << ' '
                                        << b << '\n';

    return 0;
}
