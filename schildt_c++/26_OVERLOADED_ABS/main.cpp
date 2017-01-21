#include <iostream>
using namespace std;

int myabs(int i);
double myabs(double d);
long myabs(long l);

int main (void)
{
   cout << myabs(-10) << "\n";
   cout << myabs(-11.0) << "\n";
   cout << myabs(-9L) << "\n";

   return 0;
}

int myabs(int i)
{
   cout << "Using int version of myabs(): ";
   if(i < 0)
      return -i;
   else
      return i;
}

double myabs(double d)
{
   cout << "Using double version of myabs(): ";
   if (d < 0.0)
      return -d;
   else
      return d;
}

long myabs(long l)
{
   cout << "Using long version of myabs(): ";
   if (l < 0)
      return -l;
   else
      return l;
}
