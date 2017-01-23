#include <iostream>
#include <new>
#include <cstdlib>
using namespace std;

class three_d
{
    int x, y, z;
public:
    three_d()
    {
        x = y = z = 0;
        cout << "Создание объекта 0, 0, 0\n";
    }
    three_d(int i, int j, int k)
    {
        x = i; y = j; z = k;
        cout << "Создание объекта " << i << ", ";
        cout << j << ", " << k;
        cout << '\n';
    }
    ~three_d() { cout << "Разрушение объекта\n"; }
    void *operator new(size_t size);
    void *operator new[](size_t size);
    void operator delete(void *p);
    void operator delete[](void *p);
    void show();
};

void *three_d::operator new(size_t size)
{
    void *p;
    cout << "Выделение памяти для объекта класса three_d.\n";
    p=malloc(size);
    if(!p)
    {
        bad_alloc ba;
        throw ba;
    }
    return p;
}

void *three_d::operator new[](size_t size)
{
    void *p;
    cout << "Выделение памяти для массива three_d объектов.\n";
    p = malloc(size);
    if(!p)
    {
        bad_alloc ba;
        throw ba;
    }
    return p;
}

void three_d:: operator delete(void *p)
{
    cout << "Удаление объекта класса three_d.\n";
    free(p);
}

void three_d::operator delete[](void *p)
{
    cout << "Удаление массива объектов типа three_d.\n";
    free(p);
}

void three_d::show()
{
    cout << x << ", ";
    cout << y << ", ";
    cout << z << "\n";
}

int main()
{
    three_d *p1, *p2;

    try
    {
        p1 = new three_d[3];
        p2 = new three_d(5, 6, 7);
    }
    catch (bad_alloc ba)
    {
        cout << "Ошибка при выделении памяти.\n";
        return 1;
    }

    p1[1].show();
    p2->show();

    delete [] p1;
    delete p2;

    return 0;
}
