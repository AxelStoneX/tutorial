#include <iostream>
using namespace std;

template<class X> void repeat (X data, int times)
{
    do{
        cout << data << "\n";
        times--;
    }while(times);
}

int main()
{
    repeat("Это тест", 3);
    repeat(100, 5);
    repeat(99.0/2, 4);

    return 0;
}
