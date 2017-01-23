#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class timer
{
	int seconds;
public:
	timer (const char *t) { seconds = atoi(t); }
	timer (int t) { seconds = t; }
	timer (int min, int sec) { seconds = min*60 + sec; }
	void run();
};

void timer::run()
{
	clock_t t1;
	t1 = clock();
	while((signed)(clock()/CLOCKS_PER_SEC - t1/CLOCKS_PER_SEC) < seconds)
		cout << "a\n";
}

int main()
{
	timer a(2), b("20"), c(1, 10);

	a.run();
	//b.run();
	//c.run();

	return 0;
}
