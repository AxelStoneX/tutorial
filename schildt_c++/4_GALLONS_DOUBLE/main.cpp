#include <iostream>
using namespace std;

int main(void)
{
   float gallons;
   float liters;

   cout << "Enter number of gallons\n";
   cin  >> gallons;
   liters = gallons * 3.7854;
   cout << "Number of liters is: " << liters << "\n";

   return 0;

}
