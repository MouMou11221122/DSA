#include<stdio.h>
#include<stdlib.h>

struct set{
	int rank;
	struct set* parent;
	int value;
};
int N, M;
struct set** disjoint_forest;

void init_set()
{
	disjoint_forest = malloc(sizeof(struct set*) * N);
	for(int i = 0; i < N; i++){
		disjoint_forest[i] = malloc(sizeof(struct set));
		disjoint_forest[i]->rank = 0;
		disjoint_forest[i]->parent = disjoint_forest[i];
		disjoint_forest[i]->value = i + 1;
	}
}

struct set* find(struct set* x)
{
	if(x->parent != x){
		x->parent = find(x->parent);
	}
	return x->parent;
}

void unionn(struct set* x, struct set* y)
{
	struct set* set1,* set2;
	set1 = find(x);
	set2 = find(y);
	if(set1 != set2){
		if(set1->rank > set2->rank){
			set2->parent = set1;
		}else{
			set1->parent = set2;
			if(set1->rank == set2->rank) set2->rank = set2->rank + 1;	
		}
	}
}

void print_path(struct set* x)
{
		int counter = 0;
		while(x->parent != x){
			if(counter == 0){
				printf("%d", x->value);
			}else{
				printf(" %d", x->value);
			}
			++counter;
			x = x->parent;
		}
		if(counter > 0)	printf(" ");
		printf("%d\n", x->value);
}


int main(int argc, char** argv)
{
		char action;
		int para1, para2;
		scanf("%d", &N);
		scanf("%d", &M);
		init_set();
		for(int i = 0; i < M; i++){
			scanf("\n%c", &action);
			if(action == 'F'){	/* find	*/
				scanf("%d", &para1);
				find(disjoint_forest[para1 - 1]);
			}else if(action == 'U'){	/* union */
				scanf("%d%d", &para1, &para2);
				unionn(disjoint_forest[para1 - 1], disjoint_forest[para2 - 1]);
			}else if(action == 'P'){	/* print */
				scanf("%d", &para1);
				print_path(disjoint_forest[para1 - 1]);
			}
		}
		exit(0);
}
