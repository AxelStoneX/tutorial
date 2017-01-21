#include "Matrix.h"
int write_bin_matrix(char *fname,float *a,int n,int m)
{
	FILE *fp;
	fp=fopen(fname,"wb");
	if(fp==NULL)
		return 1;
	if(fwrite(&n,sizeof(int),1,fp)!=1)
		return 2;
	if(fwrite(&m,sizeof(int),1,fp)!=1)
		return 2;
	if(fwrite(a,sizeof(float),n*m,fp)!= n*m)
		return 3;
	fclose(fp);
	return 0;
}
float *read_bin_matrix(char *fname, int *n, int *m)
{
	int i;
	float *a;
	FILE *fp;
	fp=fopen(fname,"rb");
	if(fp==NULL)
		return NULL;
	i=fread(n,sizeof(int),1,fp);
	if(i<1)
		goto err;
	i=fread(m,sizeof(int),1,fp);
	if(i<1)
		goto err;
	a=malloc((*n)*(*m)*sizeof(float));
	if(a==NULL)
		goto err;
	i=fread(a,sizeof(float),(*n)*(*m),fp);
	if(i<(*n)*(*m))
		goto err2;
	fclose(fp);
	return a;
err2: free(a);
err:  fclose(fp);
	  return NULL;
}
int write_txt_matrix(char *fname,float *a,int n,int m)
{
	int i,j;
	FILE *fp;
	fp=fopen(fname,"w");
	if(fp==NULL)
		return 1;
	fprintf(fp,"%d ",n);
	fprintf(fp,"%d \n",m);
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			fprintf(fp,"%f\t",a[i*m+j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
}

float *read_txt_matrix(char *fname, int *n, int *m)
{
	int i,j;
	float *a;
	FILE *fp;
	fp=fopen(fname,"r");
	if(fp==NULL)
		return NULL;
	fscanf(fp,"%d",n);
	if(m<=0)
		goto err;
	fscanf(fp,"%d",m);
	if(n<=0)
		goto err;
	a=malloc((*n)*(*m)*sizeof(float));
	if(a==NULL)
		goto err;
	for(i=0;i<(*n);i++)
		for(j=0;j<(*m);j++)
			if(fscanf(fp,"%f",&(a[i*(*m)+j]))==NULL)
				goto err2;
	fclose(fp);
	return a;
err2: free(a);
err:  fclose(fp);
	  return NULL;
}

float *input_matrix(int n, int m)
{
	float *a;
	int i,j;
	a=malloc(n*m*sizeof(float));
	if(a==NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
		{
			printf("Enter the %d-th %d-th element of matrix\n",i,j);
			scanf("%f",&a[i*m+j]);
		}
	return a;
}
void output_matrix(float *a,int n, int m)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			fprintf(stdout,"%f\t",a[i*m+j]);
		printf("\n");
	}
}

float *rnd_matrix(int n, int m)
{
	float *a;
	int i,j,s;
	s=time(NULL);
	srand(s);
	if((a=malloc(n*m*sizeof(float)))==NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
			a[i*m+j] = (rand()%100)/1.24;
	return a;
}

float *matrix_sum(float *a, float *b, int n, int m, int k, int l)
{
	int i,j;
	float *c;
	if(n!=k || m!= l)
		return NULL;
	if((c=malloc(n*m*sizeof(float))) == NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
			c[i*m+j]=a[i*m+j]+b[i*m+j];
	return c;
}

float *matrix_multiplication(float *a, float *b, int n, int m, int k, int l)
{
	int i,j;
	float *c;
	if(m!= k)
		return NULL;
	if((c=malloc(n*l*sizeof(float)))==NULL)
		return NULL;
	for(i=0;i<n;i++)
		for(j=0;j<l;j++)
		{
			c[i*l+j]=0;
			for(k=0;k<m;k++)
				c[i*l+j]+=a[i*m+k]*b[k*l+j];
		}
	return c;
}




