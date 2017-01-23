#include <iostream>
using namespace std;

#define mkstr(s) # s

int main()
{
    cout << mkstr(I like C++);
    return 0;
}
