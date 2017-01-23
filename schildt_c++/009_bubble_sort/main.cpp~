#include <iostream>
#include <cstdlib>
using namespace std;

int size = 10;

int main(void)
{
   int num[size];
   int i, a, b, t;
   //Заполнение массива
   for(i=0; i<size; i++)
       num[i] = rand();
   //Вывод на экран несортированного массива
   cout << "Unsorted array:\n";
   for(i=0; i < size; i++)
      cout << num[i] << " ";
   cout << "\n";
   //Пузырьковая сортировка
   for(a=1; a<size; a++)
      for(b=size-1; b>=a; b--)
         if(num[b-1] > num[b]);
         {
            t = num[b];
            num[b] = num[b-1];
            num[b-1] = t;
         }
   //Вывод на экран сортированного массива
   cout << "Sorted array:\n";
   for(i=0; i < size; i++)
      cout << num[i] << " ";
   cout << "\n";
}
