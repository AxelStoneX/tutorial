#include <iostream>
#include <cstdlib>
using namespace std;

template <class AType, int size>
class atype
{
    AType a[size];
public:
    atype(){
        register int i;
        for(i=0; i<size; i++) a[i] = i;
    }
    AType &operator[](int i);
};

template <class AType, int size>
AType &atype<AType, size>::operator[](int i)
{
    if(i < 0 || i > size-1) {
        cout << "\nЗначение индекса ";
        cout << i << " за пределами границ массива.\n";
        exit(1);
    }
    return a[i];
}

int main()
{
    atype<int, 10> intob;
    atype<double,15> doubleob;

    int i;

    cout << "Массив целых чисел: ";
    for(i=0; i<10; i++) intob[i] = i;
    for(i=0; i<10; i++) cout << intob[i] << " ";
    cout << '\n';

    cout << "Массив double значений: ";
    for(i=0; i<15; i++) doubleob[i] = (double) i/3;
    for(i=0; i<15; i++) cout << doubleob[i] << " ";
    cout << "\n";

    intob[12] = 100;

    return 0;
}
