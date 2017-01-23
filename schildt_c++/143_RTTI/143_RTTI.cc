#include <iostream>
#include <typeinfo>
using namespace std;

class Base
{
    virtual void f() {};
};

class Derived1: public Base {};

class Derived2: public Base {};

int main()
{
    Base *p, baseob;
    Derived1 ob1;
    Derived2 ob2;

    p = &baseob;
    cout << "Переменная p указывает на объект типа ";
    cout << typeid(*p).name() << endl;

    p = &ob1;
    cout << "Переменная p указывает на объект типа ";
    cout << typeid(*p).name() << endl;

    p = &ob2;
    cout << "Переменная p указывает на объект типа ";
    cout << typeid(*p).name() << endl;
}
