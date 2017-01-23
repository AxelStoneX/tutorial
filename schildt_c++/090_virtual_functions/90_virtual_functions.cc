#include <iostream>
using namespace std;

class base
{
public:
	virtual void who(){
		cout << "Базовый класс.\n";
	}
};

class first_d : public base
{
public:
	void who(){
		cout << "Первый производный класс.\n";
	}
};

class second_d : public base
{
public:
	void who(){
		cout << "Второй производный класс.\n";
	}
};

int main()
{
	base base_obj;
	base *p;
	first_d first_obj;
	second_d second_obj;

	p = &base_obj;
	p->who();

	p = &first_obj;
	p->who();

	p = &second_obj;
	p->who();

	return 0;
}
