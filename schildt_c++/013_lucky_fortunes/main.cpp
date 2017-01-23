#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int kbhit(void);

char *fortunes[] = {
   "Soon money will flow to you like a river.\n",
   "New love will shine at your life.\n",
   "You will live long and happy.\n",
   "Money, invested in  buisness, will make profit in future.\n",
   "Close friend will be looking for your favour.\n" 
};

int main(void)
{
   int chance;
   
   cout << 
      "To learn youe fate press any key.\n";

   while(!kbhit()) rand();

   cout << "\n";

   chance = rand();
   chance = chance % 5;
   cout << fortunes[chance];

   return 0;
}
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
