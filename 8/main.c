#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node
{
    int value;
    int next;
};


int N;
struct node * list = NULL;

int main(int argc, char ** argv)
{
    scanf("%d", &N);
    list = malloc(sizeof(struct node) * (N + 1));
    for(int i = 1; i <= N; i++)
    {
        scanf("%d%d", &(list[i].value), &(list[i].next));
    }

    //cycle detection
    int tortoise = 1, hare = 1;
    bool firstMeet = true;

    while(tortoise != 0 && list[tortoise].next != 0)
    {
        if(!firstMeet)
        {
            printf(" ");
        }
        printf("%d", list[tortoise].value);
        if(list[tortoise].value == list[hare].value)
        {
            if(firstMeet)
            {
                firstMeet = false;
                if(list[tortoise].next == tortoise)
                {

		    printf(" %d", list[tortoise].value);
                    printf("\n");
                    exit(0);
                }
            }else{
                printf("\n");
                exit(0);
            }
        }    
        tortoise = list[list[tortoise].next].next;
        hare = list[hare].next;
    }

    if(tortoise == 0)
    {
        printf("\n");
    }else if(list[tortoise].next == 0)
    {
        if(!firstMeet)
        {
            printf(" ");
        }
        printf("%d\n", list[tortoise].value);
    }

    exit(0);
}
