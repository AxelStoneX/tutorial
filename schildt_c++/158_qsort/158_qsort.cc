#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

int comp(const void *a, const void *b);

int main()
{
    char str[] = "pointers to functions give flexibility";
    qsort(str, strlen(str), 1, comp);
    cout << "Отсортированная строка: " << str;
    return 0;
}

int comp(const void *a, const void *b)
{
    return *(char *) a - *(char *)b;
}
