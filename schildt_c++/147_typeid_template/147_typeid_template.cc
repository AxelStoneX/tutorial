#include <iostream>
#include <cstdlib>
using namespace std;

template <class T> class figure
{
public:
    T x, y;
    figure(T i, T j)
    {
        x = i;
        y = j;
    }
    virtual T area() = 0;
};

template <class T> class triangle: public figure<T>
{
public:
    triangle(T i, T j) : figure<T>(i, j) {}
    T area() { return this->x * 0.5 * this->y; }
};

template <class T> class rectangle : public figure<T>
{
public:
    rectangle(T i, T j) : figure<T>(i, j){}
    T area() { return this->x * this->y; }
};

template <class T> class circle : public figure<T>
{
public:
    circle(T i, T j=0) : figure<T>(i, j) {}
    T area() { return 3.14 * this->x * this->x; }
};

figure<double> *generator()
{
    switch(rand() % 3)
    {
    case 0: return new circle<double>(10.0);
    case 1: return new triangle<double>(10.1, 5.3);
    case 2: return new rectangle<double>(4.3, 5.7);
    }
    return 0;
}

int main()
{
    figure<double> *p;
    int i;
    int t=0, c=0, r=0;

    for(i=0; i<10; i++)
    {
        p = generator();
        cout << "Объект имеет тип " << typeid(*p).name();
        cout << ". ";

        if(typeid(*p) == typeid(triangle<double>)) t++;
        if(typeid(*p) == typeid(rectangle<double>)) r++;
        if(typeid(*p) == typeid(circle<double>)) c++;

        cout << "Площадь равна " << p->area() << endl;
    }

    cout << endl;
    cout << "Сгенерированы такие объекты:\n";
    cout << " треугольников: " << t << endl;
    cout << " прямоугольников: " << r << endl;
    cout << " кругов: " << c << endl;

    return 0;
}
