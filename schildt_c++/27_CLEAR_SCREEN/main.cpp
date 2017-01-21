#include <iostream>
using namespace std;

void clrscr(int size=25);

int main()
{
   int i;
   
   for(i=0; i<30; i++)
      cout << i << '\n';
   clrscr();

   for(i=0; i<30; i++)
      cout << i << '\n';
   clrscr(10);

   return 0;
}

void clrscr(int size)
{
   for(; size; size--)
      cout << '\n';
}
