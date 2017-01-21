#include "TAS.h"
int main(void)
{
	int n;
	int a[MAX_LEN];
	char fname[MAX_LEN];
	printf("Enter size of your array\n");
	scanf("%d",&n);
	printf("Enter name of the file\n");
	scanf("%s",fname);
	if(input_array(n,a))
	{
		printf("Error with count of elements\n");
		return 1;
	}
	if(writet_array(fname,a,n))
	{
		printf("Error opening the file\n");
		return 2;
	}
	return 0;
}

