#include <iostream>
using namespace std;

void showflags(ios::fmtflags f);

int main()
{
    ios::fmtflags f;
    f = cout.flags();
    showflags(f);
    cout.setf(ios::showpos);
    cout.setf(ios::scientific);

    f = cout.flags();
    showflags(f);

    cout.unsetf(ios::scientific);

    f = cout.flags();
    showflags(f);

    return 0;
}

void showflags(ios::fmtflags f)
{
    long i;

    for(i=0x4000; i; i = i >> 1)
        if(i & f) cout << "1 ";
        else      cout << "0 ";
    cout << "\n";
}
