#include <iostream>
using namespace std;

int main(void)
{
   int a,b;
   cout << "Enter first integer: ";
   cin >> a;
   cout << "Enter second integer: ";
   cin >> b;
   if(a > b)
      cout << a << " > " << b << "\n";
   else if(a < b)
      cout << a << " < " << b << "\n";
   else if(a == b)
      cout << a << " = " << b << "\n";
   else
   {
      cout << "Something went wrong\n";
      return -1;
   }
   return 0;
}
