#include "Forest.h"
void print_tree(struct t_node *root)
{
	if(root!=NULL)
	{
		print_tree(root->left);
		printf("%d ",root->key);
		print_tree(root->right);
	}
}
/*Функция добавления элемента в бинарное дерево.
На входе:
- корень дерева struct t_node *root
- число int a, которое необходимо добавить
На выходе:
- Коды ошибок
Коды ошибок:
- 0 - всё хорошо
- 1 - ошибки выделения памяти
*/
int add_tree(struct t_node *root,int a)
{
	struct t_node *leaf;
	if(root->key >= a)
	{
		if(root->left!=NULL)
			add_tree(root->left,a);
		else
		{
			if((leaf=malloc(sizeof(struct t_node)))==NULL)
				return 1;
			leaf->left=NULL;
			leaf->right=NULL;
			leaf->key=a;
			root->left=leaf;
			return 0;
		}
	}
	else
	{
		if(root->right!=NULL)
			add_tree(root->right,a);
		else
		{
			if((leaf=malloc(sizeof(struct t_node)))==NULL)
				return 1;
			leaf->left=NULL;
			leaf->right=NULL;
			leaf->key=a;
			root->right=leaf;
			return 0;
		}
	}
}
/*
Функция поиска минимума в бинарном дереве
На входе:
- корень дерева struct t_node *root
На выходе:
- значение найденного минимума cur->key
*/
int min_finding(struct t_node *root)
{
	struct t_node *cur;
	cur=root;
	while(cur->left!=NULL)
		cur=cur->left;
	return cur->key;
}
/*
Функция поиска минимума в бинарном дереве
На входе:
- корень дерева struct t_node *root
На выходе:
- значение найденного минимума cur->key
*/
int max_finding(struct t_node *root)
{
	struct t_node *cur;
	cur=root;
	while(cur->right != NULL)
		cur=cur->right;
	return cur->key;
}
int k_rank_min_finding(struct t_node *root)
{
	return ;
}
int k_rank_max_finding(struct t_node *root)
{
	return ;
}
/*
Функция поиска элемента в бинарном дереве
На входе:
- корень дерева struct t_node *root
- число int a, которое нужно найти 
На выходе
- 0, если элемент найден
- 1, если элемент не найден
*/
int element_finding(struct t_node *root, int a)
{
	struct t_node *cur;
	cur = root;
	if(cur->key > a)
	{
		if(cur->left == NULL)
			return 1;
		cur=cur->left;
		if((element_finding(cur,a))==0)
			return 0;
	}
	if(cur->key < a)
	{
		if(cur->right == NULL)
			return 1;
		cur=cur->right;
		if((element_finding(cur,a))==0)
			return 0;
	}
	if(cur->key == a)
		return 0;
}
/*Функция нахождения элемента 2
На входе:
- корень дерева struct t_node *root
- число int a, которое нужно найти
- указатели struct t_node *cur и struct t_node *prev, в которые в неявном виде
будут записанны указатели на найденный и впередиидущий элементы дерева
На выходе
- 0, если элемент найден
- 1, если элемент не найден
*/
int element_finding_2(struct t_node *root,int a, struct t_node **cur,struct t_node **prev)
{
	(*cur)=root;
	(*prev)=root->left;
	while(((*cur)->key != a) && ((*cur)!=NULL))
	{
		(*prev)=(*cur);
		if((*cur)->key < a)
			(*cur)=(*cur)->right;
		else
			(*cur)=(*cur)->left;
	}
	if((*cur)->left == NULL)
		return 1;
	return 0;
}
/*Функция удаления элемента из бинарного дерева
На входе:
- корень дерева struct t_node *root
- число, которое нужно удалить int a
На выходе:
- корень дерева struct t_node *root
- NULL, если элемент не удалось найти и удалить
*/
/*struct t_node *element_deliting(struct t_node *root,int a)
{
	struct t_node *cur=root;//=root;
	struct t_node *prev=NULL;//=root->left;
	struct t_node *p=NULL;
	struct t_node *save=NULL;
	if(element_finding_2(root,a,cur,prev)==1)
		return NULL;
	if((cur==root) && (prev!=NULL))
	{
		root=prev;
		return root;
	}
	if(prev==NULL)
	{
		root=cur->right;
		return root;
	}
	if(prev->right == cur)
	{
		if(cur->left != NULL && cur->right != NULL)
		{
			save=cur->left;
			prev->right=cur->right;
			p=prev;
			while(p->left != NULL)
				p=p->left;
			p->left=save;
		}
		if(cur->right == NULL)
			prev->right=cur->left;
		if(cur->left == NULL)
			prev->right=cur->right;
	}
	if(prev->left == cur)
	{
		if(cur->left != NULL && cur->right != NULL)
		{
			save=cur->right;
			prev->left=cur->left;
			p=prev;
			while(p->right != NULL)
				p=p->right;
			p->right=save;
		}
		if(cur->left == NULL)
			prev->left=cur->right;
		if(cur->right == NULL)
			prev->left=cur->left;
	}
	return root;
}*/
struct t_node *element_deliting(struct t_node *root, int a)
{
	struct t_node *cur=NULL;
	struct t_node *prev=NULL;
	struct t_node *p;
	struct t_node *save;
	if(element_finding_2(root,a,&cur,&prev)==1)
		return NULL;
	if(cur==root)
	{
		if(cur->left==NULL)
		{
			root=root->right;
			return root;
		}
		if(cur->right==NULL)
		{
			root=root->left;
			return root;
		}
		p=root->right;
		while(p->left!=NULL)
			p=p->left;
		p->left=root->left;
		return root->right;
	}
	if(prev->right==cur)
	{
		save=cur->left;
		p=cur;//->right;
		while(p->left!=NULL)
			p=p->left;
		p->left=save;
		prev->right=cur->right;
		return root;
	}
	if(prev->left==cur)
	{
		save=cur->right;
		p=cur->left;
		while(p->right!=NULL)
			p=p->right;
		p->right=save;
		prev->left=cur->left;
		return root;
	}
}


		


