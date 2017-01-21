#include "TAS.h"
int main(void)
{
	int *a;
	int n;
	printf("Enter count of array's elemnts\n");
	scanf("%d",&n);
	if(!(a=rand_malloc_input_array(n)))
	{
		printf("There was some errors with memory\n");
		return 1;
	}
	output_array(n,a);
	return 0;
}
