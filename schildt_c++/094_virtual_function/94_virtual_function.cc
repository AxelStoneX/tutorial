#include <iostream>
using namespace std;

class figure {
protected:
	double x, y;
public:
	void set_dim(double i, double j=0) {
		x = i;
		y = j;
	}
	virtual void show_area() {
		cout << "Для этого класса выражение вычисления ";
		cout << "площади не определено.\n";
	}
};

class triangle : public figure {
public:
	void show_area() {
		cout << "Треугольник с высотой ";
		cout << x << " и основанием " << y;
		cout << " имеет площадь ";
		cout << x * 0.5 * y << ".\n";
	}
};

class rectangle : public figure {
public:
	void show_area() {
		cout << "Прямоугольник с размерами ";
		cout << x << " x " << y;
		cout << " имеет площадь ";
		cout << x * y << ".\n";
	}
};

class circle : public figure {
public:
	void show_area() {
		cout << "Круг с радиусом ";
		cout << x;
		cout << " имеет площадь ";
		cout << 3.14 * x * x << ".\n";
	}
};

int main()
{
	figure *p;

	triangle t;
	rectangle r;
	circle c;

	p = &t;
	p->set_dim(10.0, 5.0);
	p->show_area();

	p = &r;
	p->set_dim(10.0, 5.0);
	p->show_area();

	p = &c;
	p->set_dim(9.0);
	p->show_area();

	return 0;
}
