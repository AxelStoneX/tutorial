#include <iostream>
#include <typeinfo>
using namespace std;

class myclass
{
public:
    int a, b;
};

int main()
{
    int i, j;
    float f;
    myclass ob;

    cout << "Тип переменной i: " << typeid(i).name();
    cout << endl;
    cout << "Тип переменной f: " << typeid(f).name();
    cout << endl;
    cout << "Тип переменной ob: " << typeid(ob).name();
    cout << "\n\n";

    if(typeid(i) == typeid(j))
        cout << "Типы переменных i и j одинаковы.\n";

    if(typeid(i) != typeid(f))
        cout << "Типы переменных i и f неодинаковы.\n";

    return 0;

}
