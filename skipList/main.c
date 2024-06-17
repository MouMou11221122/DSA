#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SENTINEL_STATE 5
#define STATION_STATE 6

#define SLOW_GET 1
#define FAST_GET 2
#define INSERT 3
#define REMOVE 4

struct station
{
    struct station * prev;
    struct station * next;
    long long value;
};

struct skipListNode
{
    struct station *stationArray;   //stationArray[0] is use to store the address of skipList node itself
    int stationArrayCount;
    long long value;                //value is used to store the real value
    struct skipListNode * next;
};

struct outputListNode
{
    long long value;
    struct outputListNode * next;
};

//search output list info
struct outputListNode * outputListHead = NULL;
struct outputListNode * outputListTail = NULL;


//operation info
long long targetNum;

//sentinel info
int sentinelArrayCount;
//int sentinelArrayLimit;
struct station ** sentinelArray = NULL;

//skip list node info
int skipListNodeCount = 0;
struct skipListNode * skipListNodeHead = NULL, * skipListNodeTail = NULL;

//traverse info
struct station ** traversePointer1 = NULL;
struct station * traversePointer2 = NULL;

//insert info
struct station ** insertPreviousArray = NULL;

//delete info
struct station ** deletePreviousArray = NULL;

void sentinelArrayInit()
{
    sentinelArrayCount = 0;
    sentinelArray = malloc(sizeof(struct station *) * 70);
}

struct skipListNode * directConstructNewSkipListNode(int layer)
{
    struct skipListNode * newNode;
    newNode = malloc(sizeof(struct skipListNode));
    newNode -> stationArrayCount = layer + 1;
    newNode -> next = NULL;
    newNode -> stationArray = malloc(sizeof(struct station) * (layer + 1));
    newNode -> stationArray -> next = NULL;
    newNode -> stationArray -> prev = NULL;
    newNode -> stationArray -> value = (long long)newNode;
    newNode -> value = targetNum;
    for(int i = 1; i <= layer; i++)
    {
        (newNode -> stationArray + i) -> prev = NULL;
        (newNode -> stationArray + i) -> next = NULL;
        (newNode -> stationArray + i) -> value = targetNum;
    }
    return newNode;
}

bool coinFlip(int layer)
{       //IsOdd(ceilling(k / 2 ^ (layer - 1)))  k : long long
    return (targetNum >> (layer - 1)) & 0x1;
}

void ins()
{   //insert the targetNum   
    int insertNumConstructToWhichLayer = 0;
    struct skipListNode * newNode;
    int oldSentinelArrayCount = sentinelArrayCount;
    while(coinFlip(insertNumConstructToWhichLayer + 1))
    {
        insertNumConstructToWhichLayer++;
    }
    newNode = directConstructNewSkipListNode(insertNumConstructToWhichLayer);

    if((insertNumConstructToWhichLayer + 1) > sentinelArrayCount)
    {
        sentinelArrayCount = insertNumConstructToWhichLayer + 1;
    }

    if(skipListNodeCount == 0)
    {
        skipListNodeHead = skipListNodeTail = newNode;
        for(int i = 0; i <= insertNumConstructToWhichLayer; i++)
        {
            sentinelArray[i] = newNode -> stationArray + i;
        }
        skipListNodeCount++;
        return;

    }else{
        int currentLayer;
        int state = SENTINEL_STATE;
        //insertPreviousArray = malloc(sizeof(struct station *) * oldSentinelArrayCount);
        traversePointer1 = sentinelArray + oldSentinelArrayCount - 1;
        for(currentLayer = oldSentinelArrayCount - 1; currentLayer >= 0; currentLayer--)
        {
            if(state == SENTINEL_STATE)
            {
                if(currentLayer != 0)
                {
                    if(targetNum <= (*traversePointer1) -> value)
                    {
                        state = STATION_STATE;
                        traversePointer2 = *traversePointer1;
                        while(traversePointer2 -> next != NULL && targetNum <= traversePointer2 -> next -> value)
                        {
                            traversePointer2 = traversePointer2 -> next;
                        }
                        //insertPreviousArray[currentLayer] = traversePointer2;
                        //transitionLayer = currentLayer;
                        if(currentLayer <= insertNumConstructToWhichLayer)
                        {
                            (newNode -> stationArray + currentLayer) -> next = traversePointer2 -> next;
                            (newNode -> stationArray + currentLayer) -> prev = traversePointer2;
                            (newNode -> stationArray + currentLayer) -> prev -> next = newNode -> stationArray + currentLayer;
                            if((newNode -> stationArray + currentLayer) -> next != NULL)
                            {
                                (newNode -> stationArray + currentLayer) -> next -> prev = newNode -> stationArray + currentLayer;
                            }
                        }
                        traversePointer2--;
                    }else{
                        if(currentLayer <= insertNumConstructToWhichLayer)
                        {
                            (newNode -> stationArray + currentLayer) -> next = *traversePointer1;
                            if((newNode -> stationArray + currentLayer) -> next != NULL)
                            {
                                (newNode -> stationArray + currentLayer) -> next -> prev = newNode -> stationArray + currentLayer;
                            }
                            *traversePointer1 = newNode -> stationArray + currentLayer;
                        }
                        traversePointer1--;
                    }
                }else{
                    if(targetNum <= ((struct skipListNode *)((*traversePointer1) -> value)) -> value)
                    {
                        state = STATION_STATE;
                        traversePointer2 = *traversePointer1;
                        while(traversePointer2 -> next != NULL && targetNum <= ((struct skipListNode *)(traversePointer2 -> next -> value)) -> value)
                        {
                            traversePointer2 = traversePointer2 -> next;
                        }

                        (newNode -> stationArray + currentLayer) -> next = traversePointer2 -> next;
                        (newNode -> stationArray + currentLayer) -> prev = traversePointer2;
                        (newNode -> stationArray + currentLayer) -> prev -> next = newNode -> stationArray + currentLayer;
                        if((newNode -> stationArray + currentLayer) -> next != NULL)
                        {
                            (newNode -> stationArray + currentLayer) -> next -> prev = newNode -> stationArray + currentLayer;
                        }
                        
                        newNode -> next = ((struct skipListNode *)(traversePointer2 -> value)) -> next;
                        ((struct skipListNode *)(traversePointer2 -> value)) -> next = newNode;
                        if(((struct skipListNode *)(traversePointer2 -> value))  == skipListNodeTail)
                        {
                            skipListNodeTail = skipListNodeTail -> next;
                        }

                    }else{
                        (newNode -> stationArray + currentLayer) -> next = *traversePointer1;
                        if((newNode -> stationArray + currentLayer) -> next != NULL)
                        {
                            (newNode -> stationArray + currentLayer) -> next -> prev = newNode -> stationArray + currentLayer;
                        }
                        *traversePointer1 = newNode -> stationArray + currentLayer;
                        newNode -> next = skipListNodeHead;
                        skipListNodeHead = newNode;
                    }
                }

            }else{
                if(currentLayer != 0)
                {
                    while(traversePointer2 -> next != NULL && targetNum <= traversePointer2 -> next -> value)
                    {
                        traversePointer2 = traversePointer2 -> next;
                    }
                    if(currentLayer <= insertNumConstructToWhichLayer)
                    {
                        (newNode -> stationArray + currentLayer) -> next = traversePointer2 -> next;
                        (newNode -> stationArray + currentLayer) -> prev = traversePointer2;
                        (newNode -> stationArray + currentLayer) -> prev -> next = newNode -> stationArray + currentLayer;
                        if((newNode -> stationArray + currentLayer) -> next != NULL)
                        {
                            (newNode -> stationArray + currentLayer) -> next -> prev = newNode -> stationArray + currentLayer;
                        }
                    }
                }else{
                    while(traversePointer2 -> next != NULL && targetNum <= ((struct skipListNode *)(traversePointer2 -> next -> value)) -> value)
                    {
                        traversePointer2 = traversePointer2 -> next;
                    }
                    (newNode -> stationArray + currentLayer) -> next = traversePointer2 -> next;
                    (newNode -> stationArray + currentLayer) -> prev = traversePointer2;
                    (newNode -> stationArray + currentLayer) -> prev -> next = newNode -> stationArray + currentLayer;
                    if((newNode -> stationArray + currentLayer) -> next != NULL)
                    {
                        (newNode -> stationArray + currentLayer) -> next -> prev = newNode -> stationArray + currentLayer;
                    }
                        
                    newNode -> next = ((struct skipListNode *)(traversePointer2 -> value)) -> next;
                    ((struct skipListNode *)(traversePointer2 -> value)) -> next = newNode;
                    if(((struct skipListNode *)(traversePointer2 -> value))  == skipListNodeTail)
                    {
                        skipListNodeTail = skipListNodeTail -> next;
                    }
                }
                                
                traversePointer2--;
            }
        }

        if(insertNumConstructToWhichLayer > oldSentinelArrayCount - 1)
        {
            for(currentLayer = oldSentinelArrayCount; currentLayer <= insertNumConstructToWhichLayer; currentLayer++)
            {
                sentinelArray[currentLayer] = newNode -> stationArray + currentLayer;
            }
            
        }
        skipListNodeCount++;
    }

}


void fastGet()
{   //get the targetNum and print the path
    if(skipListNodeCount == 0)
    {
        printf("-1\n");
        return;
    }else{
        //bool firstPrint = true;
        int currentLayer;
        int state = SENTINEL_STATE;
        //struct outputListNode * tmp;
        traversePointer1 = sentinelArray + sentinelArrayCount - 1;
        for(currentLayer = sentinelArrayCount - 1; currentLayer >= 0; currentLayer--)
        {
            if(state == SENTINEL_STATE)
            {
                if(currentLayer != 0)
                {
                    if(targetNum <= (*traversePointer1) -> value)
                    {
                        state = STATION_STATE;
                        traversePointer2 = *traversePointer1;
                        printf("%lld", traversePointer2 -> value);
                        //firstPrint = false;
                        //outputListHead = outputListTail = malloc(sizeof(struct outputListNode));
                        //outputListTail -> value = traversePointer2 -> value;
                        //outputListTail -> next = NULL;

                        while(traversePointer2 -> next != NULL && targetNum <= traversePointer2 -> next -> value)
                        {
                            traversePointer2 = traversePointer2 -> next;
                            printf(" %lld", traversePointer2 -> value);
                            //outputListTail -> next = malloc(sizeof(struct outputListNode));
                            //outputListTail -> next -> value = traversePointer2 -> value;
                            //outputListTail -> next -> next = NULL;
                            //outputListTail = outputListTail -> next;
                        }
                        traversePointer2--;
                    }else{
                        traversePointer1--;
                    }
                }else{  //current layer == 0
                    struct skipListNode * tmp = (struct skipListNode *)((*traversePointer1) -> value); 
                    if(targetNum <= tmp -> value)
                    {
                        state = STATION_STATE;
                        printf("%lld", tmp -> value);
                        //firstPrint = false;
                        //outputListHead = outputListTail = malloc(sizeof(struct outputListNode));
                        //outputListTail -> value = tmp -> value;
                        //outputListTail -> next = NULL;
                        while(tmp -> next != NULL && targetNum <= tmp -> next -> value)
                        {
                            tmp = tmp -> next;
                            printf(" %lld", tmp -> value);
                            //outputListTail -> next = malloc(sizeof(struct outputListNode));
                            //outputListTail -> next -> value = tmp -> value;
                            //outputListTail -> next -> next = NULL;
                            //outputListTail = outputListTail -> next;
                        }
                    }
                }
            }else{  //state == STATION_STATE
                if(currentLayer != 0)
                {
                    //outputListTail -> next = malloc(sizeof(struct outputListNode));
                    //outputListTail -> next -> value = traversePointer2 -> value;
                    //outputListTail -> next -> next = NULL;
                    //outputListTail = outputListTail -> next;
                    printf(" %lld", traversePointer2 -> value);
                    while(traversePointer2 -> next != NULL && targetNum <= traversePointer2 -> next -> value)
                    {
                        traversePointer2 = traversePointer2 -> next;
                        printf(" %lld", traversePointer2 -> value);
                        //outputListTail -> next = malloc(sizeof(struct outputListNode));
                        //outputListTail -> next -> value = traversePointer2 -> value;
                        //outputListTail -> next -> next = NULL;
                        //outputListTail = outputListTail -> next;
                    }
                    traversePointer2--;
                }else{  //currentLayer == 0
                    struct skipListNode * tmp = (struct skipListNode *)(traversePointer2 -> value);
                    printf(" %lld", tmp -> value);
                    //outputListTail -> next = malloc(sizeof(struct outputListNode));
                    //outputListTail -> next -> value = tmp -> value;
                    //outputListTail -> next -> next = NULL;
                    //outputListTail = outputListTail -> next;
                    while(tmp -> next != NULL && targetNum <= tmp -> next -> value)
                    {
                        tmp = tmp -> next;
                        printf(" %lld", tmp -> value);
                        //outputListTail -> next = malloc(sizeof(struct outputListNode));
                        //outputListTail -> next -> value = tmp -> value;
                        //outputListTail -> next -> next = NULL;
                        //outputListTail = outputListTail -> next;
                    }
                }
            }
        }
        
        if(state == STATION_STATE)
        {
            printf("\n");
        }else{
            printf("-1\n");
        }
    }

}

void rm_assist_routine(int currentLayer)
{
    int deleteTraverseLayer;

    deleteTraverseLayer = currentLayer;
    while(traversePointer2 -> prev == NULL && traversePointer2 -> next == NULL && deleteTraverseLayer > 0)
    {
        sentinelArray[deleteTraverseLayer] = NULL;
        sentinelArrayCount--;
        deleteTraverseLayer--;
        traversePointer2--;

    }

    for(; deleteTraverseLayer >= 1; deleteTraverseLayer--)
    {
        if(traversePointer2 -> prev != NULL)
        {
            traversePointer2 -> prev -> next = traversePointer2 -> next;
            if(traversePointer2 -> next != NULL)
            {
                traversePointer2 -> next -> prev = traversePointer2 -> prev;
            }
        }else{
            sentinelArray[deleteTraverseLayer] = traversePointer2 -> next;
            traversePointer2 -> next -> prev = NULL;
        }
        traversePointer2--;
    }

    //deleteTraverseLayer arrives at layer 0
    struct skipListNode * tmp, *tmpPre;
    tmpPre = NULL;
    tmp = (struct skipListNode *)(traversePointer2 -> value);
    if(traversePointer2 -> prev != NULL)
    {
        tmpPre = (struct skipListNode *)(traversePointer2 -> prev -> value);
    }
    if(tmp == skipListNodeHead)
    {
        if(tmp == skipListNodeTail)
        {
            skipListNodeHead = skipListNodeTail = NULL;
            sentinelArrayCount = 0;
        }else{
            tmp -> stationArray -> next -> prev = NULL;
            sentinelArray[0] = tmp -> stationArray -> next;
            skipListNodeHead = skipListNodeHead -> next;
        }
    }else{
        if(tmp == skipListNodeTail)
        {
            tmpPre -> stationArray -> next = NULL;
            tmpPre -> next = NULL;
            skipListNodeTail = tmpPre;
        }else{
            tmp -> stationArray -> next -> prev = tmp -> stationArray -> prev;
            tmp -> stationArray -> prev -> next = tmp -> stationArray -> next;
            tmpPre -> next = tmp -> next;
        }
    }
    free(tmp);
    skipListNodeCount--;
}


void rm_assist_routine2(struct skipListNode ** tmp)
{
    if(*tmp == skipListNodeHead)
    {
        if(*tmp == skipListNodeTail)
        {
            skipListNodeHead = skipListNodeTail = NULL;
            sentinelArrayCount--;
        }else{
            sentinelArray[0] = (*tmp) -> next -> stationArray;
            (*tmp) -> next -> stationArray -> prev = NULL;
            skipListNodeHead = (*tmp) -> next;
        }
    }else{
        struct skipListNode * tmpPre = (struct skipListNode *)((*tmp) -> stationArray -> prev -> value);
        if(*tmp == skipListNodeTail)
        {
            skipListNodeTail = tmpPre;
            tmpPre -> stationArray -> next = NULL;
            tmpPre -> next = NULL;
        }else{
            tmpPre -> stationArray -> next = (*tmp) -> next -> stationArray;
            if((*tmp) -> next -> stationArray != NULL)
            {
                (*tmp) -> next -> stationArray -> prev = tmpPre -> stationArray;
            }
            tmpPre -> next = (*tmp) -> next;
        }
    }
    free(*tmp);
    if(skipListNodeCount == 1)
    {
        sentinelArrayCount--;
    }
    skipListNodeCount--;
}

void rm()
{   //remove the targetNum
    if(skipListNodeCount > 0)
    {   
        //int deleteSituation = DELETE_SITUALTION_ONE;
        //bool found = false;
        int state = SENTINEL_STATE;
        int currentLayer;
        traversePointer1 = sentinelArray + sentinelArrayCount - 1;
        //deletePreviousArray = malloc(sizeof(struct station *) * sentinelCount);        
        for(currentLayer = sentinelArrayCount - 1; currentLayer >= 0; currentLayer--)
        {
            if(state == SENTINEL_STATE)
            {
                if(currentLayer != 0)
                {
                    if(targetNum <= (*traversePointer1) -> value)
                    {
                        state = STATION_STATE;
                        traversePointer2 = *traversePointer1;
                        if(targetNum == traversePointer2 -> value)
                        {                            
                            rm_assist_routine(currentLayer);
                            return;
                        }

                        while(traversePointer2 -> next != NULL && targetNum <= traversePointer2 -> next -> value)
                        {
                            traversePointer2 = traversePointer2 -> next;
                            if(targetNum == traversePointer2 -> value)
                            {
                                rm_assist_routine(currentLayer);
                                return;
                            }
                        }
                        traversePointer2--;
                    }else{
                        traversePointer1--;
                    }
                }else{  //currentLayer == 0
                    struct skipListNode * tmp = (struct skipListNode *)((*traversePointer1) -> value);
                    for(; tmp != NULL && targetNum <= tmp -> value; tmp = tmp -> next)
                    {
                        if(tmp -> value == targetNum)
                        {
                            rm_assist_routine2(&tmp);    
                            return;
                        }
                    }
                }
            }else{  //state == STATION_STATE
                if(currentLayer != 0)
                {
                    while(traversePointer2 -> next != NULL && targetNum <= traversePointer2 -> next -> value)
                    {
                        traversePointer2 = traversePointer2 -> next;
                        if(targetNum == traversePointer2 -> value)
                        {
                            rm_assist_routine(currentLayer);
                            return;
                        }
                    }
                    traversePointer2--;
                }else{
                    struct skipListNode * tmp = (struct skipListNode *)(traversePointer2 -> value);
                    while(tmp -> next != NULL && targetNum <= tmp -> next -> value)
                    {
                        tmp = tmp -> next;
                        if(targetNum == tmp -> value)
                        {
                            rm_assist_routine2(&tmp);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void slowGet()
{
    struct skipListNode * tmp = skipListNodeHead;
    if(skipListNodeCount == 0 || targetNum > tmp -> value)
    {
        printf("-1\n");
        return;
    }else{
        for(;;)
        {
            printf("%lld", tmp -> value);
            if(tmp -> next != NULL && targetNum <= tmp -> next -> value)
            {
                printf(" ");
            }else{
                printf("\n");
                return;
            }
            tmp = tmp -> next;
        }
    }
}

int main(int argc, char ** argv)
{   
    int i, type;
    int operationNums;

    sentinelArrayInit();

    scanf("%d", &operationNums);
    for(i = 0; i < operationNums; i++)
    {
        scanf("%d%lld", &type, &targetNum);
        if(type == SLOW_GET){
            slowGet();
        }else if(type == FAST_GET){
            fastGet();
        }else if(type == INSERT){
            ins();
        }else if(type == REMOVE){
            rm();
        }
    }
    exit(0);
}