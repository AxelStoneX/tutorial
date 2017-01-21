//#include <stdio.h>
#include "Fifo.h"
int main(void)
{
	int lang;
	int n;            //Длина посадочной полосы
	int cur_t=0;      //Текущее абсолбтное время(в минутах)
	int com_n=0;      //Общее кол-во самолётов, подлетевших к аэропорту
	int go_away=0;    //Общее кол-во самолётов, отправившихся в аэропорт
	int success_land_n=0;  //Кол-во самолётов, удачно совершивших посадку
	int wait_n=0;     //Кол-во самолётов, побывавших в очереди
	int wait_t=0;     //Общее время ожидания
	int lost=0;       //Кол-во самолётов, не успевших сесть на момент закрытия. Отправляются в другой аэропорт
	int c;            //Интервал времени до прилёта следующего самолёта
	int w;            //Время обслуживания самолёта на посадочной полосе
	int s;
	int temp;
	int er=0;
	double temp1;
	setlocale( LC_CTYPE, ".1251" );
	s=time(NULL);
	srand(s);
	printf("===========Airport simulator v.0.1.1.1===============\n");
	printf("==========Written by Alexander Tsukanov==============\n\n\n");
	printf("What language do you want to use\n(Выберите язык программы)\n1 - for english\n2 - для русского языка\n");
	scanf("%d",&lang);
	printf("\n");
	if(lang!=1 && lang!=2)
	{
		printf("You pressed wrong key\n(Вы нажали не ту кнопку)\n");
		return 1;
	}
	if(lang==1)
		printf("The airport works three days a week without breaks and dinners\n");
	else
		printf("Аэропорт работает три дня в неделю круглосуточно без обеда и без перерывов\n");
	if(lang==1)
		printf("Enter length of landing string\n");
	else
		printf("Введите длину посадочной полосы\n");
	scanf("%d",&n);
	if(n<0)
	{
		if(lang==1)
		{
			printf("You must be wrong.\n");
			return 2;
		}
		else
		{
			printf("Длина посадочной полосы не может быть отрицательным числом\n");
			return 3;
		}
	}
	Fifo coming(n);                  //Список из интервалов между прилётами самолётов
	Fifo waiting(n);                 //Список из промежутков времени обслуживания каждого самолёта
begin:
	while(cur_t < TIME)
	{
		c=0+rand()%(20-0+1);
		w=15+rand()%(30-15+1);
		if(waiting.is_empty()==1)//Если на посадочной полосе никого нет, самолёт не ждёт в очереди на посадку
		{
			coming.add(c);
			waiting.add(w);
			com_n++;
			success_land_n++;
			cur_t+=c;
			goto begin;
		}
		if(waiting.is_full()==1 && c<waiting.first())//Если очередь полна, то самолёт улетает в другой аэропорт
		{
			waiting.head_change(-c);
			go_away++;
			com_n++;
			cur_t+=c;
			goto begin;
		}
		if(c < waiting.first())//Если самолёт на посадочной полосе будет ремонтироваться дольше, чем время до прилёта следующего самолёта
		{
			waiting.head_change(-c);
			coming.add(c);
			waiting.add(w);
			cur_t+=c;
			com_n++;
			wait_n++;
			wait_t+=w;
		}
		if(c==waiting.first())//Если самолёт на посадочной полосе улетит в тот же момент времени, что и прилетит следующий, то самолёт успевает встать в очередь
		{
			coming.deleting(&er);
			waiting.deleting(&er);
			coming.add(c);
			waiting.add(w);
			com_n++;
			success_land_n++;
			wait_n++;
			wait_t+=w;
			cur_t+=c;
		}
		if(c>waiting.first())//Если самолёт на посадочной полосе освободит её раньше, чем прилетит новый самолёт
		{
			temp=c-waiting.first();
			coming.deleting(&er);
			waiting.deleting(&er);
			coming.add(c);
			waiting.add(w);
			waiting.head_change(-temp);
			com_n++;
			success_land_n++;
			wait_n++;
			wait_t+=w;
			cur_t+=c;
		}
	}
	lost=waiting.get_n();
	wait_t-=waiting.sum();
	temp1=wait_t/(wait_n-lost);
	if(lang==1)
	{
		printf("Work has been completed\n  ==Statistics:==  \n");
		printf("Completely count of airplanes:  %d\n",++com_n);
		printf("Successfully landed airplanes:  %d\n",success_land_n++);
		printf("Sended to other airport:        %d\n",(go_away+lost));
		printf("Among them lated airplanes:     %d\n",lost);
		printf("Completely time of expection:   %d minutes\n",wait_t);
		printf("Standed in queue airplanes:     %lf\n",wait_n-lost);
		printf("Middle time of expection:       %lf minutes\n",temp1);
	}
	else
	{
		printf("Работа аэропорта была завершена\n  ==Статистика:==  \n");
		printf("Общее число самолётов:                            %d\n",++com_n);
		printf("Успешно приземлившиеся самолёты:                  %d\n",success_land_n++);
		printf("Отправленные в другой аэропорт самолёты:          %d\n",go_away+lost);
		printf("Среди них отправленные по закрытии аэропорта:     %d\n",lost);
		printf("Общее время ожидания в очереди на посадку:        %d минут\n",wait_t);
		printf("Побывавшие в очереди самолёты:                    %d\n",wait_n-lost);
		printf("Среднее время ожидания:                           %lf minutes\n",temp1);
	}
	return 0;
}



/*int main(void)
{
	int n;
	int k;
	int a;
	printf("Enter count of elements\n");
	scanf("%d",&n);
	Fifo turn(n);
	while(1)
	{
		printf("Select\n1 - for adding\n2 - for deliting\n3 - for full checking\n4 - for exit\n");
		scanf("%d",&k);
		if(k==1)
		{
			printf("Enter element\n");
			while(scanf("%d",&a)!=EOF)
				if(turn.add(a)==1)
				{
					printf("Sorry\nYour queue is full\n");
					printf("Enter element\n");
				}
				else
				{
					turn.print();
					printf("Enter element\n");
				}
		}
		if(k==2)
		{
			int er=0;
			a=turn.deleting(&er);
			if(er==1)
				printf("Sorry\n Your queue is empty\n");
			else
			{
				printf("Deleted elment: %d\n",a);
				printf("Now your queue is:\n");
				turn.print();
			}
		}
		if(k==3)
		{
			if(turn.is_full() == 1)
				printf("Your queue is full\n");
			else
				printf("There still space\n");
		}
		if(k==4)
			break;
	}
	printf("Finally\n");
	turn.print();
	return 0;
}*/
























































