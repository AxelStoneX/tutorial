#include <iostream>
using namespace std;

int main(void)
{
   int j,k;
   int &i = j;

   j = 10;

   cout << j << " " << i;
   k = 121;
   i = k;
   cout << "\n" << j << "\n";

   return 0;
}
