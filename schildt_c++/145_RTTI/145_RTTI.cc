#include <iostream>
#include <cstdlib>
using namespace std;

class figure
{
protected:
    double x, y;
public:
    figure(double i, double j)
    {
        x = i;
        y = j;
    }
    virtual double area() = 0;
};

class triangle: public figure
{
public:
    triangle(double i, double j) : figure(i, j) {}
    double area() { return x * 0.5 * y; }
};

class rectangle : public figure
{
public:
    rectangle(double i, double j) : figure(i, j) {}
    double area() { return x * y; }
};

class circle : public figure
{
public:
    circle(double i, double j=0) : figure(i, j) {}
    double area() { return 3.14*x*x; }
};

figure *factory()
{
    switch(rand() % 3)
    {
    case 0: return new circle(10.0);
    case 1: return new triangle(10.1, 5.3);
    case 2: return new rectangle(4.3, 5.7);
    }
    return 0;
}

int main()
{
    figure *p;
    int i;

    int t=0, r=0, c=0;

    for(i=0; i<10; i++)
    {
        p = factory();
        cout << "Объект имеет тип " << typeid(*p).name();
        cout << ". ";
        if(typeid(*p) == typeid(triangle)) t++;
        if(typeid(*p) == typeid(rectangle)) r++;
        if(typeid(*p) == typeid(circle)) c++;

        cout << "Площадь равна " << p->area() << endl;
    }

    cout << endl;
    cout << "Сгенерированы такие объекты:\n";
    cout << "  треугольников: " << t << endl;
    cout << "  прямоугольников: " << r << endl;
    cout << "  кругов: " << c << endl;
    cout << "  Конец программы.\n";

    return 0;
}
