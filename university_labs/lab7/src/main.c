#include "Forest.h"
int main(void)
{
	struct t_node *root;
	struct t_node *nroot;
	int a;
	int n;
	int i;
	int k;
	printf("Enter the 1 -th element. Press Ctrl+Z to stop tree input\n");
	if(scanf("%d",&a)==-1)
	{
		printf("Your tree is empty\n");
		return 0;
	}
	if((root=malloc(sizeof(struct t_node)))==NULL)
	{
		printf("Error of tree creating\n");
		return 1;
	}
	root->key=a;
	root->left=NULL;
	root->right=NULL;
	printf("Enter the element of tree\n");
	while((scanf("%d",&a)!=-1))
	{
		if((add_tree(root,a))!=0)
		{
			printf("There was some errors with memory allocating\n");
			return 2;
		}
		printf("Your tree now:\n\n");
		print_tree(root);
		printf("\n\n");
		printf("Enter the element of tree\n");
	}
	printf("Finally,your tree is:\n");
	print_tree(root);
	printf("\n\n");
	while(1)
	{
		printf("\nWhat do you want to do ?\n1 - for min finding\n2 - for max finding\n3 - for k-rank min finding\n4 - for k-rank max finding\n5 - for element finding\n6 - to elemnt deliting\n7 - to close application\n");
		scanf("%d",&a);
		if(a==1)
			printf("Minimum in this tree is %d\n",min_finding(root));
		if(a==2)
			printf("Maximum in this tree is %d\n",max_finding(root));
		if(a==3)
		{
			printf("Coming soon...\n");
			k_rank_min_finding(root);
		}
		if(a==4)
		{
			printf("Coming soon...\n");
			k_rank_max_finding(root);
		}
		if(a==5)
		{
			printf("What element do you want to find?\n");
			if(scanf("%d",&k)==-1)
			{
				printf("You pressed wrong key\n");
				break;
			}
			if(element_finding(root,k)==0)
				printf("Success!There is such element in tree!\n");
			else
				printf("Sorry.There is no such element in tree!\n");
		}
		if(a==6)
		{
			printf("What element do you want to delete\n");
			if(scanf("%d",&k)==-1)
			{
				printf("You pressed wrong key\n");
				break;
			}
			if((nroot=element_deliting(root,k))==NULL)
			{
				printf("There is no such element\n");
				break;
			}
			root=nroot;
			print_tree(root);
			break;
		}
		if(a==7)
			return 0;
		if(a!=1 && a!=2 && a!=3 && a!=4 && a!=5 && a!=6)
		{
			printf("You pressed wrong key!\n");
			break;
		}
	}
	return 0;
}
