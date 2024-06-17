#include<stdio.h>
#include<stdlib.h>

#define RED		0
#define BLACK	1

struct node{
	char color;
	int value;
	struct node* left_child;
	struct node* right_child;
	struct node* parent;
};
int node_count, action_count;
int black_depth, depth;
struct node** preorder;

struct node* construct_rbtree(int left, int right)
{
	struct node* root;
	if(left > right)	return NULL;
	root = preorder[left];
	if(left < right){
		int mid = left + 1;
		while(mid <= right && root->value >= preorder[mid]->value)	++mid;
		root->left_child = construct_rbtree(left + 1, mid - 1);
		root->right_child = construct_rbtree(mid, right);
	}
	if(root->left_child) root->left_child->parent = root;
	if(root->right_child) root->right_child->parent = root;
	return root;
}

struct node* search_rb_node(struct node* root, int value)
{
	struct node* traverse_pointer = root;
	depth = 0;
	black_depth = 0;
	while(traverse_pointer->value != value){
		++depth;
		if(traverse_pointer->color == BLACK) ++black_depth;
		if(traverse_pointer->value < value)	traverse_pointer = traverse_pointer->right_child;
		else traverse_pointer = traverse_pointer->left_child;
	}
	return traverse_pointer;
}

void rb_left_rotate(struct node** root, int value)
{  
    /* find the node first  */
    struct node* x, * y;    
    x = search_rb_node(*root, value);
    y = x->right_child;
    x->right_child = y->left_child;
    if(y->left_child != NULL){
        y->left_child->parent = x;
    }
    y->parent = x->parent;
    if(x->parent != NULL && x == x->parent->right_child){
        x->parent->right_child = y;
    }else if(x->parent != NULL && x == x->parent->left_child){
        x->parent->left_child = y;
    }else{
        *root = y;
    }
    x->parent = y;
    y->left_child = x;
} 

void rb_right_rotate(struct node** root, int value)
{  
    /* find the node first  */
    struct node* x, * y;
    x = search_rb_node(*root, value);
    y = x->left_child;
    x->left_child = y->right_child;
    if(y->right_child != NULL){
        y->right_child->parent = x;
    }
    y->parent = x->parent;
    if(x->parent != NULL && x == x->parent->right_child){
        x->parent->right_child = y;
    }else if(x->parent != NULL && x == x->parent->left_child){
        x->parent->left_child = y;
    }else{
        *root = y;
    }
    x->parent = y;
    y->right_child = x;
}

int main(int argc, char** argv)
{
	struct node* root;
	scanf("%d", &node_count);
	preorder = malloc(sizeof(struct node*) * node_count);
	for(int i = 0; i < node_count; ++i){
		char color;
		int value;
		preorder[i] = malloc(sizeof(struct node));
		scanf("\n%c%d", &color, &value);
		preorder[i]->color = color == 'R' ? RED : BLACK;
		preorder[i]->value = value;
		preorder[i]->parent = preorder[i]->left_child = preorder[i]->right_child = NULL;
	}
	root = construct_rbtree(0, node_count - 1);
	scanf("%d", &action_count);
	for(int i = 0; i < action_count; ++i){
		char action;
		int para;
		scanf("\n%c%d", &action, &para);
		if(action == 'P'){
			struct node* x = search_rb_node(root, para);
			int lc, rc;
			lc = x->left_child ? x->left_child->value : -1;
			rc = x->right_child ? x->right_child->value : -1;
			printf("%d %d %d %d\n", lc, rc, black_depth, depth);
		}else if(action == 'L'){
			rb_left_rotate(&root, para);
		}else if(action == 'R'){
			rb_right_rotate(&root, para);
		}
	}
	exit(0);
}
