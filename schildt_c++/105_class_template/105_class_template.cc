#include <iostream>
using namespace std;

template <class T>
class myclass {
    T x;
public:
    myclass(T a) {
        cout << "В теле обобщенного класса myclass.\n";
        x = a;
    }
    T getx() { return x; }
};

template <>
class myclass<int> {
    int x;
public:
    myclass(int a) {
        cout << "В теле специализации myclass<int>.\n";
        x = a * a;
    }
    int getx() { return x; }
};

int main()
{
    myclass<double> d(10.1);
    cout << "double: " << d.getx() << "\n\n";

    myclass<int> i(5);
    cout << "int: " << i.getx() << "\n";

    return 0;
}
