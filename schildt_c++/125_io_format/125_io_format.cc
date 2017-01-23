#include <iostream>
using namespace std;

int main()
{
    cout.setf(ios::showpos);
    cout.setf(ios::scientific);
    cout << 123 << " " << 123.23 << "\n";

    cout.precision(2);
    cout.width(10);
    cout << 123 << " ";
    cout.width(10);
    cout << 123.23 << "\n";

    cout.fill('#');
    cout.width(10);
    cout << 123 << " ";
    cout .width(10);
    cout << 123.23;

    return 0;
}
