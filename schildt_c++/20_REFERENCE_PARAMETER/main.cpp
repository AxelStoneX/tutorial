#include <iostream>
using namespace std;

void swap (int &x, int &y);

int main(void)
{
   int i, j;

   i = 10;
   j = 20;

   cout << "Old value of i = " << i
        << " j = " << j << "\n";

   swap(i, j);

   cout << "New value of i = " << i
        << " j = " << j << "\n";
   return 0;
}

void swap(int &x, int &y)
{
   int temp;

   temp = x;
   x = y;
   y = temp;
}
