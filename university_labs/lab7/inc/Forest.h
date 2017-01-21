#include <stdio.h>
struct t_node
{
	int key;
	struct t_node *left;
	struct t_node *right;
};
void print_tree(struct t_node *root);

int add_tree(struct t_node *root,int a);

int min_finding(struct t_node *root);

int max_finding(struct t_node *root);

int k_rank_min_finding(struct t_node *root);

int k_rank_max_finding(struct t_node *root);

int element_finding(struct t_node *root, int a);

int element_finding_2(struct t_node *root,int a, struct t_node **cur,struct t_node **prev);

struct t_node *element_deliting(struct t_node *root,int a);
