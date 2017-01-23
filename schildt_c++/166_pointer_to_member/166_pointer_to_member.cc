#include <iostream>
using namespace std;

class myclass
{
public:
    int sum;
    void sum_it(int x);
};

void myclass::sum_it(int x)
{
    int i;
    sum = 0;
    for(i=x; i; i--) sum += i;
}

int main()
{
    int myclass::*dp;
    void (myclass::*fp)(int x);
    myclass *c, d;

    c = &d;

    dp = &myclass::sum;
    fp = &myclass::sum_it;

    (c->*fp)(7);

    cout << "Сумма чисел от 1 до 7 равна: " << c->*dp;

    return 0;
}
