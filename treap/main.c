#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define GET_MIN(a, b) 		((a) < (b) ? (a) : (b))
#define GET_MAX(a, b) 		((a) > (b) ? (a) : (b))
#define DELETE				1
#define INSERT 				2
#define SUM					3
#define LAZY				1
#define NOT_LAZY			0

struct node{
	long long value;
	long long sum;
	long long priority;
	int size;						/*	total node number of tree/treap rooted at this node	*/
	struct node* left_child;
	struct node* right_child;
	int lazy;
};

struct treap_pairs{
	struct node* left_treap;
	struct node* right_treap;
};

int N, T;
struct node* left_treap_root, * right_treap_root;
struct node* tmp_treap_root;
bool first;
int node_count;

long long get_priority()
{
	return (long long)rand();
}

struct node* create_node(long long value)
{
	struct node* new_node;
	new_node = malloc(sizeof(struct node));
	if(new_node == NULL){
		fprintf(stderr, "malloc error\n");
		exit(1);
	}
	new_node->value = value;
	new_node->sum = value;
	new_node->priority = get_priority();
	new_node->size = 1;
	new_node->left_child = NULL;
	new_node->right_child = NULL;
	new_node->lazy = NOT_LAZY;
	return new_node;
}

/*	key stands for the location in the sequence	*/
struct treap_pairs* treap_split(struct node* root, int key)
{
	struct treap_pairs* tp;
	struct treap_pairs* tmp_tp;
	tp = malloc(sizeof(struct treap_pairs));
	if(!root){
		tp->left_treap = tp->right_treap = NULL;
		return tp;
	}
	/*	count the node number of leftchild including root itself	*/
	int left_child_size = root->left_child ? root->left_child->size + 1 : 1;				
	if(left_child_size <= key){		
		tp->left_treap = root;
		tmp_tp = treap_split(root->right_child, key - left_child_size);
		root->right_child = tmp_tp->left_treap;
		tp->right_treap = tmp_tp->right_treap;
	}else{
		tp->right_treap = root;
		tmp_tp = treap_split(root->left_child, key);
		root->left_child = tmp_tp->right_treap;
		tp->left_treap = tmp_tp->left_treap;
	}
	root->sum = root->value;
	root->size = 1;
	if(root->left_child){
		root->sum += root->left_child->sum;
		root->size += root->left_child->size;
	}
	if(root->right_child){
		root->sum += root->right_child->sum;
		root->size += root->right_child->size;
	}
	free(tmp_tp);
	return tp;
}

/*
struct node* treap_merge(struct node* x, struct node* y)
{
	if(!x || !y)	return x | y;
	int x_kth = x->left_child ? x->left_child->size + 1 : 1;
	int y_kth = y->left_child ? y->left_child->size + 1 : 1;
	if(x->priority >= y->priority && x_kth > y_kth){
		x->left_child = treap_merge(x->left_child, y);
		return x;
	}else if(x->priority >= y->priority && x_kth < y_kth){
		x->right_child = treap_merge(x->right_child, y);
		return x;
	}else if(x->priority < y->priority && x_kth > y_kth){
		y->right_child = treap_merge(y->right_child, x);
		return y;
	}else{
		y->left_child = treap_merge(y->left_child, x);
		return y;
	}
}
*/

struct node* treap_merge(struct node* x, struct node* y)
{
	if(x == NULL || y == NULL){
		if(x && x->lazy == LAZY)
		{	
			x->lazy = NOT_LAZY;
			if(x->left_child && x->left_child->lazy == LAZY) x->left_child->lazy = NOT_LAZY;
		}
		if(y && y->lazy == LAZY)
		{
			y->lazy = NOT_LAZY;
			if(y->left_child && y->left_child->lazy == LAZY) y->left_child->lazy = NOT_LAZY;
		}
		return x == NULL ? y : x;
	}
	struct node* lazy_node,* non_lazy_node;
	if(x->lazy == LAZY){
		lazy_node = x;
		non_lazy_node = y;
	}else{
		lazy_node = y;
		non_lazy_node = x;
	}
	if(lazy_node->left_child) lazy_node->left_child->lazy = LAZY;
	if(lazy_node->priority >= non_lazy_node->priority){
		lazy_node->left_child = treap_merge(non_lazy_node, lazy_node->left_child);
		lazy_node->lazy = NOT_LAZY;
		lazy_node->sum = lazy_node->value;
		lazy_node->size = 1;
		if(lazy_node->left_child){
			lazy_node->sum += lazy_node->left_child->sum;
			lazy_node->size += lazy_node->left_child->size;
		}
		if(lazy_node->right_child){
			lazy_node->sum += lazy_node->right_child->sum;
			lazy_node->size += lazy_node->right_child->size;
		}
		return lazy_node;
	}else{
		non_lazy_node->right_child = treap_merge(lazy_node, non_lazy_node->right_child);
		non_lazy_node->sum = non_lazy_node->value;
		non_lazy_node->size = 1;
		if(non_lazy_node->left_child){
			non_lazy_node->sum += non_lazy_node->left_child->sum;
			non_lazy_node->size += non_lazy_node->left_child->size;
		}
		if(non_lazy_node->right_child){
			non_lazy_node->sum += non_lazy_node->right_child->sum;
			non_lazy_node->size += non_lazy_node->right_child->size;
		}
		return non_lazy_node;
	}
}

/*	insert the node with the value after kth	*/
void treap_insert(struct node** root, int kth, long long value)
{
	if(!*root){
		*root = create_node(value);
	}else{
		struct treap_pairs* tmp_tp;
		struct node* a,* b, *c;
		a = create_node(value);
		a->lazy = LAZY;
		tmp_tp = treap_split(*root, kth);
		b = treap_merge(a, tmp_tp->left_treap);
		if(tmp_tp->right_treap) tmp_tp->right_treap->lazy = LAZY;
		c = treap_merge(b, tmp_tp->right_treap);
		*root = c;
		free(tmp_tp);
	}
}

void treap_delete(struct node** root, int kth)
{
	struct node* a, *b, *c;
	struct treap_pairs* tmp_tp;
	tmp_tp = treap_split(*root, kth);
	a = tmp_tp->left_treap;
	c = tmp_tp->right_treap;
	free(tmp_tp);
	tmp_tp = treap_split(a, kth - 1);
	a = tmp_tp->left_treap;
	b = tmp_tp->right_treap;
	free(b);
	if(c) c->lazy = LAZY;
	*root = treap_merge(a, c);
	free(tmp_tp);
}

long long treap_sum(struct node* root, int l, int r, int absolute_left_bound, int absolute_right_bound)
{
	if(l == absolute_left_bound && r == absolute_right_bound)	return root->sum;
	if(r < absolute_left_bound || l > absolute_right_bound) return 0;
	if(l < absolute_left_bound) l = absolute_left_bound;
	if(r > absolute_right_bound) r = absolute_right_bound;
	long long sum = 0;
	int absolute_root_position = root->left_child ? absolute_left_bound + root->left_child->size : absolute_left_bound;
	if(absolute_root_position >= l && absolute_root_position <= r) sum += root->value;
	if(l < absolute_root_position) sum += treap_sum(root->left_child, l, GET_MIN(r, absolute_root_position - 1), absolute_left_bound, absolute_root_position - 1);
	if(r > absolute_root_position) sum += treap_sum(root->right_child, GET_MAX(l, absolute_root_position + 1), r, absolute_root_position + 1, absolute_right_bound);
	return sum;
}

void inorder_traversal(struct node* root)
{
	if(root){
		inorder_traversal(root->left_child);
		if(first){
			printf("%lld", root->value);
			first = false;
		}else{
			printf(" %lld", root->value);
		}
		inorder_traversal(root->right_child);
	}
}

/*	
 *		We implement the merge-split treap.
 *		It is a 1-based sequence.
 */
int main(int argc, char** argv)
{
	struct node* treap_root = NULL;
	scanf("%d%d", &N, &T);								/*	N indicates N integers a1,a2,...,aN. T indicates T operations.	*/

	for(int i = 0; i < N; ++i){							/*  read the N inputs.	*/
		long long value;
		scanf("%lld", &value);
		treap_insert(&treap_root, i, value);
		++node_count;
	}
	
	for(int i = 0; i < T; ++i){
		int operation;
		scanf("%d", &operation);
		if(operation == DELETE){
			int kth;
			scanf("%d", &kth);
			treap_delete(&treap_root, kth);
			--node_count;
		}else if(operation == INSERT){
			int kth;
			long long value;
			scanf("%d%lld", &kth, &value);
			treap_insert(&treap_root, kth, value);
			++node_count;
		}else{
			int left, right;
			scanf("%d%d", &left, &right);
			printf("%lld\n", treap_sum(treap_root, left, right, 1, node_count));
		}
	}
	printf("%d\n", node_count);
	first = true;
	inorder_traversal(treap_root);
	printf("\n");
	exit(0);
}
