#include <iostream>
using namespace std;

class ShareVar
{
    static int num;
public:
    void setnum(int i) { num = i; }
    void shownum() { cout << num << " "; }
};

int ShareVar::num;

int main()
{
    ShareVar a, b;

    a.shownum();
    b.shownum();

    a.setnum(10);

    a.shownum();
    b.shownum();

    return 0;
}
