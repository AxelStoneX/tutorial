#include <iostream>
using namespace std;

int main()
{
    int i;
    char *p = "Это короткая строка.";
    i = reinterpret_cast<int> (p);
    cout << i;
    return 0;
}
