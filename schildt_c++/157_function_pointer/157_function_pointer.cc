#include <iostream>
using namespace std;

void vline(int i), hline(int i);

int main()
{
    void (*p)(int i);
    p = vline;
    (*p)(4);
    p = hline;
    (*p)(3);
    return 0;
}

void hline(int i)
{
    for(;i;i--) cout << "-";
    cout << "\n";
}

void vline(int i)
{
    for(; i; i--) cout << "|\n";
}
