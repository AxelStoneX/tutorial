#include <iostream>
using namespace std;

void f(int i);
void f(int i, int j);
void f(double k);

int main()
{
   f(10);
   f(10, 20);
   f(12.23);
   return 0;
}

void f(int i)
{
   cout << "In f(int) function i = " << i << "\n";
}

void f(int i, int j)
{
   cout << "In f{int, int) i = " << i
        << " j = " << j << "\n";
}

void f(double k)
{
   cout << "In f(double) k = " << k << "\n";
}
