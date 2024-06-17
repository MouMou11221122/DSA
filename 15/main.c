#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VERTEX_WHITE	11
#define VERTEX_GRAY		12
#define VERTEX_BLACK	13


/*
 *	input infos
 *	vertex is equivalent to block
 *	edge is equivalent to arrow
 *	mode is either 1 or 2
 *	There are no duplicate arrows.
 */
int edgeCount;
int vertexCount;	
int mode;

/*
 *	uninversed graph's data structure
 *	system clock is for finish time
 */
struct vertex
{
	int vertexIdentifier;
	struct vertex * next;
};
struct vertex ** adjacencyList = NULL;
struct vertex ** adjacencyListTracePointer = NULL;

unsigned char * colorArray = NULL;
int systemClock;
int * finishTime = NULL; 



/*
 *	inversed graph's data structure'
 */

struct vertex ** inversedAdjacencyList = NULL;
struct vertex ** inversedAdjacencyListTracePointer = NULL;

/*
 *	output infos
 */
int sccCount;
struct vertex ** compressedAdjacencyList = NULL;
struct vertex ** compressedAdjacencyListTracePointer = NULL;
int * sccGroup = NULL;
int * dirty = NULL;

void colorArrayInitialization()
{
	for(int i = 0; i < vertexCount; i++)
	{
		colorArray[i] = VERTEX_WHITE;
	}
}


void initializeGraph()
{
	/*
	 *	adjacencyList initialization for a reversed graph and an inreversed graph
	 */
	adjacencyList = malloc(sizeof(struct vertex *) * vertexCount);
	if(adjacencyList == NULL)
	{
		fprintf(stderr, "malloc error for adjacencyList in line %d\n", __LINE__);
		exit(1);
	}

	adjacencyListTracePointer = malloc(sizeof(struct vertex *) * vertexCount);
	if(adjacencyListTracePointer == NULL)
	{
		fprintf(stderr, "malloc error for adjacencyListTracePointer in line %d\n", __LINE__);
		exit(1);
	}

	inversedAdjacencyList = malloc(sizeof(struct vertex *) * vertexCount);
	if(inversedAdjacencyList == NULL)
	{
		fprintf(stderr, "malloc error for inversedAdjacencyList in line %d\n", __LINE__);
		exit(1);
	}

	inversedAdjacencyListTracePointer = malloc(sizeof(struct vertex *) * vertexCount);
	if(inversedAdjacencyListTracePointer == NULL)
	{
		fprintf(stderr, "malloc error for inversedAdjacencyListTracePointer in line %d\n", __LINE__);
		exit(1);
	}

	compressedAdjacencyList = malloc(sizeof(struct vertex *) * vertexCount);
	if(compressedAdjacencyList == NULL)
	{
		fprintf(stderr, "malloc error for compressedAdjacencyListTracePointer in line %d\n", __LINE__);
		exit(1);
	}

	compressedAdjacencyListTracePointer = malloc(sizeof(struct vertex *) * vertexCount);
	if(compressedAdjacencyListTracePointer == NULL)
	{
		fprintf(stderr, "malloc error for compressedAdjacencyListTracePointer in line %d\n", __LINE__);
		exit(1);
	}

	colorArray = malloc(sizeof(unsigned char) * vertexCount);
	finishTime = malloc(sizeof(int) * vertexCount);
	sccGroup = malloc(sizeof(int) * vertexCount);
	dirty = malloc(sizeof(int) * vertexCount);
	if(colorArray == NULL || finishTime == NULL || sccGroup == NULL || dirty == NULL)
	{
			fprintf(stderr, "malloc error for other data structure in line %d\n", __LINE__);
			exit(1);
	}


	for(int i = 0; i < vertexCount; i++)
	{
		adjacencyList[i] = adjacencyListTracePointer[i] = inversedAdjacencyList[i] = \
					inversedAdjacencyListTracePointer[i] = compressedAdjacencyList[i] = compressedAdjacencyListTracePointer[i] = NULL;
		colorArray[i] = VERTEX_WHITE;
		//diedVertex[i] = false;
		sccGroup[i] = dirty[i] = -1;
	}
	
	/*
	 *	construct the original graph and reversed graph simultaneously
	 */	
	int vertexIdentifierA, vertexIdentifierB;
	for(int i = 0; i < edgeCount; i++)
	{
		scanf("%d%d", &vertexIdentifierA, &vertexIdentifierB);
		if(adjacencyList[vertexIdentifierA - 1] == NULL)
		{
			adjacencyList[vertexIdentifierA - 1] = malloc(sizeof(struct vertex));
			adjacencyList[vertexIdentifierA - 1] -> vertexIdentifier = vertexIdentifierB - 1;
			adjacencyList[vertexIdentifierA - 1] -> next = NULL;
			adjacencyListTracePointer[vertexIdentifierA - 1] = adjacencyList[vertexIdentifierA - 1];			
		}else{
			adjacencyListTracePointer[vertexIdentifierA - 1] -> next = malloc(sizeof(struct vertex));
			adjacencyListTracePointer[vertexIdentifierA - 1] = adjacencyListTracePointer[vertexIdentifierA - 1] -> next;
			adjacencyListTracePointer[vertexIdentifierA - 1] -> vertexIdentifier = vertexIdentifierB - 1;
			adjacencyListTracePointer[vertexIdentifierA - 1] -> next = NULL;
		}

		if(inversedAdjacencyList[vertexIdentifierB - 1] == NULL)
		{
			inversedAdjacencyList[vertexIdentifierB - 1] = malloc(sizeof(struct vertex));
			inversedAdjacencyList[vertexIdentifierB - 1] -> vertexIdentifier = vertexIdentifierA - 1;
			inversedAdjacencyList[vertexIdentifierB - 1] -> next = NULL;
			inversedAdjacencyListTracePointer[vertexIdentifierB - 1] = inversedAdjacencyList[vertexIdentifierB - 1];			
		}else{
			inversedAdjacencyListTracePointer[vertexIdentifierB - 1] -> next = malloc(sizeof(struct vertex));
			inversedAdjacencyListTracePointer[vertexIdentifierB - 1] = inversedAdjacencyListTracePointer[vertexIdentifierB - 1] -> next;
			inversedAdjacencyListTracePointer[vertexIdentifierB - 1] -> vertexIdentifier = vertexIdentifierA - 1;
			inversedAdjacencyListTracePointer[vertexIdentifierB - 1] -> next = NULL;
		}
	}	
}





/*
 *	dfs starts from an arbitrary vertex on adjacencyList
 */
void dfs(int current_dfs_vertex)
{
	colorArray[current_dfs_vertex] = VERTEX_GRAY;
	//fprintf(stdout, "No.%d vertex be discoverd and be gray\n", current_dfs_vertex + 1);
	for(struct vertex * tracer = adjacencyList[current_dfs_vertex]; tracer != NULL; tracer = tracer -> next)
	{
		if(colorArray[tracer -> vertexIdentifier] == VERTEX_WHITE)
		{
			dfs(tracer -> vertexIdentifier);
		}
	}
	colorArray[current_dfs_vertex] = VERTEX_BLACK;
	finishTime[systemClock] = current_dfs_vertex;
	//fprintf(stdout, "No.%d vertex finishes and be black at time %d\n", current_dfs_vertex + 1, systemClock);
	++systemClock;
}

/*
 *	dfs on reversedAdjacencyList
 */
void dfs_rev(int current_dfs_vertex)
{
	sccGroup[current_dfs_vertex] = sccCount;
	colorArray[current_dfs_vertex] = VERTEX_GRAY;
	//fprintf(stdout, "No.%d vertex be discoverd and be gray\n", current_dfs_vertex + 1);
	for(struct vertex * tracer = inversedAdjacencyList[current_dfs_vertex]; tracer != NULL; tracer = tracer -> next)
	{
		if(colorArray[tracer -> vertexIdentifier] == VERTEX_WHITE)
		{
			dfs_rev(tracer -> vertexIdentifier);
		}else if(colorArray[tracer -> vertexIdentifier] == VERTEX_BLACK){
			if(sccGroup[tracer -> vertexIdentifier] < sccGroup[current_dfs_vertex])
			{	//add this edge:(sccGroup[tracer -> vertexIdentifier], sccGroup[current_dfs_vertex])
				if(dirty[tracer -> vertexIdentifier] != sccCount)
				{
					if(compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] == NULL)
					{
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] = \
								compressedAdjacencyList[sccGroup[tracer -> vertexIdentifier]] = malloc(sizeof(struct vertex));
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] -> vertexIdentifier = \
								sccGroup[current_dfs_vertex];
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] -> next = NULL;
					}else{
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] -> next = malloc(sizeof(struct vertex));
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] = \
								compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] -> next;
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] -> vertexIdentifier = \
								sccGroup[current_dfs_vertex];
						compressedAdjacencyListTracePointer[sccGroup[tracer -> vertexIdentifier]] -> next = NULL;
					}
					dirty[tracer -> vertexIdentifier] = sccCount;
				}
			}
		}
	}
	colorArray[current_dfs_vertex] = VERTEX_BLACK;
	//fprintf(stdout, "No.%d vertex finishes and be black\n", current_dfs_vertex + 1);
}

/*
 *	find scc and compress it simultaneously
 */
void find_scc_and_compress()
{
	int dfsedVertex;
	for(int i = systemClock - 1; i >= 0; i--)
	{
		dfsedVertex = finishTime[i];
		if(colorArray[dfsedVertex] == VERTEX_WHITE)
		{
			//fprintf(stdout, "start to find No.%d SCC\n", sccCount + 1);
			/*
			 *		construct the compressed adjacency list
			 */
			/*
			if(sccCount != 0)
			{	
				if(compressedAdjacencyListTracePointer[sccCount - 1] == NULL)
				{
					compressedAdjacencyList[sccCount - 1] = compressedAdjacencyListTracePointer[sccCount - 1] = \
						malloc(sizeof(struct vertex));
					compressedAdjacencyListTracePointer[sccCount - 1] -> vertexIdentifier = sccCount;
					compressedAdjacencyListTracePointer[sccCount - 1] -> next = NULL;
				}else{
					compressedAdjacencyListTracePointer[sccCount - 1] -> next = malloc(sizeof(struct vertex));
					compressedAdjacencyListTracePointer[sccCount - 1] = compressedAdjacencyListTracePointer[sccCount - 1] -> next;
					compressedAdjacencyListTracePointer[sccCount - 1] -> vertexIdentifier = sccCount;
					compressedAdjacencyListTracePointer[sccCount - 1] -> next = NULL;
				}
			}
			*/
			dfs_rev(dfsedVertex);
			sccCount++;
			//fprintf(stdout, "end to find No.%d SCC\n", sccCount);
		}
	}
}

bool test_path()
{
	bool find;
	struct vertex * test;
	if(sccCount == 1)
			return true;
	for(int i = 0; i < sccCount - 1; i++)
	{
		find = false;
		test = compressedAdjacencyList[i];
		while(test != NULL)
		{
			if(test -> vertexIdentifier == i + 1)
			{
				find = true;
				break;
			}
			test = test -> next;
		}
		if(find == false)
				return false;
	}
	return true;
}


void print_result()
{
	printf("%d\n", sccCount);

	//debug usage
	//mode = 2;

	if(mode == 2)
	{
		if(test_path())
		{
			printf("1\n");
		}else{
			printf("0\n");
		}
	}
}

int main(int argc, char ** argv)
{
	scanf("%d%d%d", &vertexCount, &edgeCount, &mode);

	//initialize the graph
	initializeGraph();
	
	/*
	 *	dfs the entire graph
	 */
	for(int i = 0; i < vertexCount; i++)
	{
		if(colorArray[i] == VERTEX_WHITE)
		{
			dfs(i);
		}
	}
	//fprintf(stdout, "system clock now : %d\n", systemClock);

	//white out the graph
	colorArrayInitialization();
	find_scc_and_compress();
	print_result();
	

	exit(0);
}

















