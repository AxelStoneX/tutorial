#include <iostream>
#include <cstdlib>
using namespace std;

int comp(const void *a, const void *b);

int main()
{
    int num[] = {10, 4, 3, 6 , 5, 7, 8 };
    int i;

    qsort(num, 7, sizeof(int), comp);
    for(i=0; i<7; i++)
        cout << num[i] << ' ';

    return 0;
}

int comp(const void *a, const void *b)
{
    return * (int *) a - * (int *) b;
}
