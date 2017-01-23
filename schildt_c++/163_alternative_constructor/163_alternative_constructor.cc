#include <iostream>
using namespace std;

class myclass
{
    const int numA;
    const int numB;
public:
    myclass(int x, int y) : numA(x), numB(y) { };
    int getNumA() { return numA; }
    int getNumB() { return numB; }
};

int main()
{
    myclass ob1(7, 9), ob2(5, 2);
    cout << "Значения членов данных объекта ob1 равны "
         << ob1.getNumB() << " и " << ob1.getNumA()
                                   << endl;

    cout << "Значения членов данных объекта ob2 равны "
         << ob2.getNumB() << " и " << ob2.getNumA()
                                   << endl;

    return 0;
}
