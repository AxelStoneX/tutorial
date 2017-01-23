#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
using namespace std;

const int SIZE = 100;

struct inv_type
{
	char   item[40];
	double cost;
	double retail;
	int    on_hand;
	int    lead_time;
} invtry[SIZE];

void enter(), init_list(), display();
void update(), input(int i);
int menu();

int main()
{
	char choice;

	init_list();

	for(;;)
	{
		choice = menu();
		switch(choice)
		{
		case 'e': enter();
			break;
		case 'd': display();
			break;
		case 'u': update();
			break;
		case 'q': return 0;
		}
	}
}

void init_list()
{
	int t;

	for(t=0; t<SIZE; t++) *invtry[t].item = '\0';
}

int menu()
{
	char ch = 'a';

	cout << '\n';
	do
	{
		cout << "(E)nter\n";
		cout << "(D)isplay\n";
		cout << "(U)pdate\n";
		cout << "(Q)uit\n\n";
		cout << "Выберите команду: ";
		cin >> ch;
	} while(!strchr("eduq", tolower(ch)));
	return tolower(ch);
}

void enter()
{
	int i;

	for(i=0; i<SIZE; i++)
		if(!*invtry[i].item) break;

	if(i==SIZE)
	{
		cout << "Список полон.\n";
		return;
	}

	input(i);
}

void input(int i)
{
	cout << "Товар: ";
	cin >> invtry[i].item;

	cout << "Стоимость: ";
	cin >> invtry[i].cost;

	cout << "Розничная цена: ";
	cin >> invtry[i].retail;

	cout << "В наличии: ";
	cin >> invtry[i].on_hand;

	cout << "Время до пополнения запасов (в днях): ";
	cin >> invtry[i].lead_time;
}

void update()
{
	int i;
	char name[80];

	cout << "Введите наименования товара: ";
	cin >> name;

	for(i=0; i<SIZE; i++)
		if(!strcmp(name, invtry[i].item)) break;

	if(i==SIZE)
	{
		cout << "Товар не найден.\n";
		return;
	}
	cout << "Введите новую информацию.\n";
	input(i);
}

void display()
{
	int t;

	for(t=0; t<SIZE; t++)
	{
		if(*invtry[t].item)
		{
			cout << invtry[t].item << '\n';
			cout << "Стоимость: $" << invtry[t].cost;
			cout << "\nВ розницу: $";
			cout << invtry[t].retail << '\n';
			cout << "В наличии: " << invtry[t].on_hand;
			cout << "\n До пополнения осталось: ";
			cout << invtry[t].lead_time << " дней\n\n";
		}
	}
}



