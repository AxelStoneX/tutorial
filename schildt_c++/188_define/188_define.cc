#include <iostream>
using namespace std;

#define MIN(a,b) (((a)<(b)) ? a : b)

int main()
{
    int x, y;

    x = 10;
    y = 20;
    cout << "Минимум равен: " << MIN(x, y);

    return 0;
}
