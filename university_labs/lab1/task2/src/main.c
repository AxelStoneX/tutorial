#include "TAS.h"
int main(void)
{
	int *a;
	int k;
	char fname[MAX_LEN];
	k=0;
	printf("Enter name of the file\n");
	scanf("%s",fname);
	if((a=readt_array(fname,&k))==NULL)
	{
		printf("There was some errors\n");
		return 1;
	}
	output_array(k,a);
	free(a);
	return 0;
}
