#include <iostream>
using namespace std;

int myfunc(void);

int main (void)
{
    cout << "Now we are in main\n";
    myfunc();
    cout << "Now we are in main again\n";

    return 0;
}

int myfunc(void)
{
    cout << "Now we are in function!!!!\n";
}
