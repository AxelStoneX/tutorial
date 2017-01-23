#include <iostream>
using namespace std;

class B {};

class D : public B {};

int main()
{
    D derived;

    try
    {
        throw derived;
    }
    catch(B b)
    {
        cout << "Перехват исключения базового класса.\n";
    }
    catch(D d)
    {
        cout << "Перехват исключения унаследованного класса.\n";
    }

    return 0;
}
