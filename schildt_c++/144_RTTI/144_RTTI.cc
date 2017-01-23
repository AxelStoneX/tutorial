#include <iostream>
#include <typeinfo>
using namespace std;

class Base
{
    virtual void f() {};
};

class Derived1: public Base {};
class Derived2: public Base {};

void WhatType(Base &ob)
{
    cout << "Параметр ob ссылается на объект типа ";
    cout << typeid(ob).name() << endl;
}

int main()
{
    Base baseob;
    Derived1 ob1;
    Derived2 ob2;

    WhatType(baseob);
    WhatType(ob1);
    WhatType(ob2);

    return 0;
}
