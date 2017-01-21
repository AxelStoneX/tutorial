#include <iostream>
using namespace std;

double &f(void);
double val = 100.0;

int main()
{
   double newval;

   cout << f() << '\n';

   newval = f();

   cout << newval << '\n';
   f() = 99.1;
   cout << f() << '\n';

   return 0;
}

double &f(void)
{
    return val;
}
