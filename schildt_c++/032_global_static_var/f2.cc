#include "32.h"

static int sum = 0, count = 0;

int r_avg(int i)
{
	sum = sum + i;
	count++;
	return sum/count;
}

void reset()
{
	sum = 0;
	count = 0;
}
