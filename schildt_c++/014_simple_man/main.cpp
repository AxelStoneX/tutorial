#include <iostream>
using namespace std;

int main (void)
{
   int choice;

   cout << "Simple operator reference:\n";
   cout << "1. for\n";
   cout << "2. if\n";
   cout << "3. switch\n";
   cout << "Enter number of section: ";
   cin  >> choice;

   switch(choice){
   case 1:
       cout << "for is the most multipurpose cycle in C++\n";
       break;
   case 2:
       cout << "if is tool for conditional branching\n";
       break;
   case 3:
       cout << "switch is tool for multidirectional branching\n";
       break;
   default:
       cout << "Please enter number in 1-3 range\n";
       break;
   }
   return 0;
}
