#include <iostream>
using namespace std;

template <class T> class myclass
{
    T a;
public:
    myclass(T i) { a = i; }
};

int main()
{
    myclass<int> o1(10), o2(9);
    myclass<double> o3(7.2);

    cout << "Объект o1 имеет тип ";
    cout << typeid(o1).name() << endl;

    cout << "Объект o2 имеет тип ";
    cout << typeid(o2).name()<< endl;

    cout << "Объект o3 имеет тип ";
    cout << typeid(o3).name() << endl;

    cout << endl;

    if(typeid(o1) == typeid(o2))
        cout << "Объекты o1 и o2 имеют одинаковый тип.\n";
    if(typeid(o1) == typeid(o3))
        cout << "Ошибка\n";
    else
        cout << "Объекты o1 и o3 имеют разные типы.\n";

    return 0;
}
