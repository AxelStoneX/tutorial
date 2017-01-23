#include <iostream>
using namespace std;

class Base
{
public:
    virtual void f() { cout << "В класс Base.\n"; }
};

class Derived : public Base
{
public:
    void f() { cout << "В классе Derived.\n"; }
};

int main()
{
    Base *bp, b_ob;
    Derived *dp, d_ob;

    dp = dynamic_cast<Derived *> (&d_ob);
    if(dp)
    {
        cout << "Приведение типов "
             << "(из Derived * в Derived *) реализовано.\n";
        dp->f();
    } else
        cout << "Ошибка\n";
    cout << endl;

    bp = dynamic_cast<Base *> (&d_ob);
    if(bp)
    {
        cout << "Приведение типов "
             << "(из Derived * в Base *) реализовано.\n";
        bp->f();
    } else
        cout << "Ошибка\n";
    cout << endl;

    bp = dynamic_cast<Base *> (&b_ob);
    if(bp)
    {
        cout << "Приведение типов "
             << "(из Base* в Base*) реализовано.\n";
        bp->f();
    } else
        cout << "Ошибка\n";
    cout << endl;

    dp = dynamic_cast<Derived *> (bp);
    if (dp)
        cout << "Ошибка\n";
    else
        cout << "Приведение типов "
             << "(из Base * в Derived *) не реализовано";
    cout << endl;

    bp = &d_ob;
    dp = dynamic_cast<Derived *> (bp);
    if(dp)
    {
        cout << "Приведение bp к типу Derived *\n"
             << "реализовано, поскольку bp действительно\n"
             << "указывает на объект класса Derived.\n";
        dp->f();
    } else
        cout << "Ошибка\n";
    cout << endl;

    bp = &b_ob;
    dp = dynamic_cast<Derived *> (bp);
    if(dp)
        cout << "Ошибка";
    else
    {
        cout << "Теперь приведение bp к типу Derived *\n"
             << "не реализовано, поскольку bp\n"
             << "в действительности указывыает на объект \n"
             << "класса Base.\n";
    }
    cout << endl;

    dp = &d_ob;
    bp = dynamic_cast<Base *> (dp);
    if(bp)
    {
        cout << "Приведение dp к типу Base * реализовано.\n";
        bp->f();
    }
    else
        cout << "Ошибка\n";

    return 0;
}
