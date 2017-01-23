#include <iostream>
using namespace std;

void f(const int *p)
{
    int *v;
    v = const_cast<int *> (p);
    *v = 100;
}

int main()
{
    int x = 99;
    cout << "Значение x до вызова функции f(): " << x
                                                 << endl;
    f(&x);
    cout <<
         "Значение x после вызова функции f(): " << x
                                                 << endl;
    return 0;
}
