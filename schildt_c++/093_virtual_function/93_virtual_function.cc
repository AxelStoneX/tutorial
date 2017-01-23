#include <iostream>
using namespace std;

class figure {
protected:
	double x, y;
public:
	void set_dim(double i, double j) {
		x = i; y = j;
	}

	virtual void show_area() {
		cout << "Для этого класса выражение вычисления ";
		cout << "площади не о пределено.\n";
	}
};

class triangle : public figure {
public:
	void show_area() {
		cout << "Треугольник с высотой ";
		cout << x << " и основанием " << y;
		cout << " имеет площадь ";
		cout << x * y << ".\n";
	}
};

class rectangle : public figure {
public:
	void show_area() {
		cout << "Прямоугольник с размерами ";
		cout << x << "x" << y ;
		cout << " имеет площадь ";
		cout << x*y << ".\n";
	}
};

int main()
{
	figure *p;
	triangle t;
	rectangle r;

	p = &t;
	p->set_dim(10.0, 5.0);
	p->show_area();

	p = &r;
	p->set_dim(10.0, 5.0);
	p->show_area();

	return 0;
}
