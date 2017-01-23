#include <iostream>
#include <cstdlib>
using namespace std;

int main(void)
{
   int magic;
   int guess;

   magic = rand();
   cout << "Enter your guess: ";
   cin >> guess;

   if(guess == magic)
      cout << "Congrats!\n";
   else
      cout << "Failed, magic number = " << magic << "\n";
   return 0; 
}
