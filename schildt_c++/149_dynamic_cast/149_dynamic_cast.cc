#include <iostream>
#include <typeinfo>
using namespace std;

class Base
{
public:
    virtual void f() {}
};

class Derived : public Base
{
public:
    void derivedOnly()
    {
        cout << "Это объект класса Derived.\n";
    }
};

int main()
{
    Base *bp, b_ob;
    Derived *dp, d_ob;

    // использование оператора typeid
    bp = &b_ob;
    if(typeid(*bp) == typeid(Derived))
    {
        dp = (Derived *) bp;
        dp->derivedOnly();
    }
    else
        cout << "Операция приведения объекта типа Base к "
             << "типу Derived не выполнилась.\n";

    bp = &d_ob;
    if(typeid(*bp) == typeid(Derived))
    {
        dp= (Derived *) bp;
        dp->derivedOnly();
    }
    else
        cout << "Ошибка, приведение типа должно "
             << "быть реализовано!\n";

    //Использование оператора dynamic_cast
    bp = &b_ob;
    dp = dynamic_cast<Derived *> (bp);
    if(dp) dp->derivedOnly();
    else
        cout << "Операция приведения объекта типа Base к "
             << "типу Derived не выполнилась.\n";
    bp = &d_ob;
    dp = dynamic_cast<Derived *> (bp);
    if (dp) dp->derivedOnly();
    else
        cout << "Ошибка, приведение типа должно "
             << "быть реализовано!\n";

    return 0;
}
