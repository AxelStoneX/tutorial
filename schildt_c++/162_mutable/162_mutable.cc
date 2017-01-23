#include <iostream>
using namespace std;

class Demo
{
    mutable int i;
    int j;
public:
    int geti() const { return i; }
    void seti(int x) const { i = x; }
};

int main()
{
    Demo ob;

    ob.seti(1900);
    cout << ob.geti();

    return 0;
}
