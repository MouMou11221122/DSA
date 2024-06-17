#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>




struct card{
    int value;
    struct card * next;
};

struct deckInfo{
    struct card * deckHeadPointer;
    struct card * deckTailPointer;
    int cardNumInThisDeck;
};

int deckCount;
int operationNum;
int type;               //1:add 2:rm 3:move 4:merge
int addedNum;
int targetDeckNumber;
int sourceDeckNumber, destinationDeckNumber;
struct card ** allDecks;
struct deckInfo * allDecksInfo;



void initAllDecksInfo(){
    for(int i = 0; i < deckCount; i++){
        //allDecksInfo[i].deckHeadPointer = allDecksInfo[i].deckTailPointer = 0x00;
        allDecksInfo[i].cardNumInThisDeck = 0;
    }
}

void add(){
    if(allDecksInfo[targetDeckNumber - 1].cardNumInThisDeck == 0){
        allDecks[targetDeckNumber - 1] = (struct card *) malloc(sizeof(struct card));
        allDecksInfo[targetDeckNumber - 1].deckTailPointer = allDecksInfo[targetDeckNumber - 1].deckHeadPointer = allDecks[targetDeckNumber - 1];
        allDecks[targetDeckNumber - 1] -> value = addedNum;
        allDecks[targetDeckNumber - 1] -> next = 0x00;
        allDecksInfo[targetDeckNumber - 1].cardNumInThisDeck++;
    }else{
        struct card * tmpDeck;
        tmpDeck = allDecksInfo[targetDeckNumber - 1].deckHeadPointer;
        allDecks[targetDeckNumber - 1] = (struct card *) malloc(sizeof(struct card));
        allDecksInfo[targetDeckNumber - 1].deckHeadPointer = allDecks[targetDeckNumber - 1];
        (allDecksInfo[targetDeckNumber - 1].deckHeadPointer) -> value = addedNum;
        (allDecksInfo[targetDeckNumber - 1].deckHeadPointer) -> next = tmpDeck;
        allDecksInfo[targetDeckNumber - 1].cardNumInThisDeck++;
    }
}


void rm(){
    if(allDecksInfo[targetDeckNumber - 1].cardNumInThisDeck == 1){
        free(allDecks[targetDeckNumber - 1]);
        allDecksInfo[targetDeckNumber - 1].deckTailPointer = allDecksInfo[targetDeckNumber - 1].deckHeadPointer = 0x00;
        //allDecks[targetDeckNumber - 1] = 0x00;
        allDecksInfo[targetDeckNumber - 1].cardNumInThisDeck--;
    }else{
        struct card * tmpDeck;
        tmpDeck = (allDecksInfo[targetDeckNumber - 1].deckHeadPointer);
        allDecksInfo[targetDeckNumber - 1].deckHeadPointer = (allDecksInfo[targetDeckNumber - 1].deckHeadPointer) -> next;
        allDecks[targetDeckNumber - 1] = allDecksInfo[targetDeckNumber - 1].deckHeadPointer;
        free(tmpDeck);
        allDecksInfo[targetDeckNumber - 1].cardNumInThisDeck--;
    }  
}


void move(){
    if(allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck > 0){
        if(allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck > 0){
            (allDecksInfo[sourceDeckNumber - 1].deckTailPointer) -> next = allDecksInfo[destinationDeckNumber - 1].deckHeadPointer;
        }
        allDecksInfo[destinationDeckNumber - 1].deckHeadPointer = allDecks[sourceDeckNumber - 1];
        allDecks[destinationDeckNumber - 1] = allDecks[sourceDeckNumber - 1];
        if(allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck == 0){
            allDecksInfo[destinationDeckNumber - 1].deckTailPointer = allDecksInfo[sourceDeckNumber - 1].deckTailPointer;
        }
        allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck += allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck;
        allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck = 0;        
    }
}


void merge(){
    struct card * leftPointer, * rightPointer, * leftNextPointer, * rightNextPointer;
    struct card * tmpPointer;
    if(allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck == 0 && allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck == 0){
        return;
    }
    if(allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck == allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck){
        leftPointer = allDecksInfo[sourceDeckNumber - 1].deckHeadPointer;
        rightPointer = allDecksInfo[destinationDeckNumber - 1].deckHeadPointer;
        tmpPointer = leftPointer;
        leftNextPointer = leftPointer;
        rightNextPointer = rightPointer;
        while(leftPointer -> next != 0x00 && rightPointer -> next != 0x00){
            leftNextPointer = leftPointer ->next;
            leftPointer -> next = rightPointer;
            rightNextPointer = rightPointer -> next;
            rightPointer -> next = leftNextPointer;
            leftPointer = leftNextPointer;
            rightPointer = rightNextPointer;
        }
        leftPointer -> next = rightPointer;
        allDecksInfo[destinationDeckNumber - 1].deckHeadPointer = allDecks[destinationDeckNumber - 1] = tmpPointer;
        allDecksInfo[destinationDeckNumber - 1].deckTailPointer = rightPointer;
        allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck += allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck;
        allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck = 0; 
    }else if(allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck < allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck){
        if(allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck != 0){
            leftPointer = allDecksInfo[sourceDeckNumber - 1].deckHeadPointer;
            rightPointer = allDecksInfo[destinationDeckNumber - 1].deckHeadPointer;
            tmpPointer = leftPointer;
            leftNextPointer = leftPointer;
            rightNextPointer = rightPointer;
            while(leftPointer -> next != 0x00 && rightPointer -> next != 0x00){
                leftNextPointer = leftPointer ->next;
                leftPointer -> next = rightPointer;
                rightNextPointer = rightPointer -> next;
                rightPointer -> next = leftNextPointer;
                leftPointer = leftNextPointer;
                rightPointer = rightNextPointer;
            }

            leftPointer -> next = rightPointer;
            allDecksInfo[destinationDeckNumber - 1].deckHeadPointer = allDecks[destinationDeckNumber - 1] = tmpPointer;
            allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck += allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck;
            allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck = 0;
        }
    }else{
        if(allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck == 0){
            move();
        }else{
            leftPointer = allDecksInfo[sourceDeckNumber - 1].deckHeadPointer;
            rightPointer = allDecksInfo[destinationDeckNumber - 1].deckHeadPointer;
            tmpPointer = leftPointer;
            leftNextPointer = leftPointer;
            rightNextPointer = rightPointer;
            while(leftPointer -> next != 0x00 && rightPointer -> next != 0x00){
                leftNextPointer = leftPointer ->next;
                leftPointer -> next = rightPointer;
                rightNextPointer = rightPointer -> next;
                rightPointer -> next = leftNextPointer;
                leftPointer = leftNextPointer;
                rightPointer = rightNextPointer;
            }
            rightPointer -> next = leftPointer -> next;
            leftPointer -> next = rightPointer;
            allDecksInfo[destinationDeckNumber - 1].deckHeadPointer = allDecks[destinationDeckNumber - 1] = tmpPointer;
            allDecksInfo[destinationDeckNumber - 1].deckTailPointer = allDecksInfo[sourceDeckNumber - 1].deckTailPointer;
            allDecksInfo[destinationDeckNumber - 1].cardNumInThisDeck += allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck;
            allDecksInfo[sourceDeckNumber - 1].cardNumInThisDeck = 0;
        }
    }
}

void printResult(){
    int i, j;
    struct card * tracePointer;
    for(i = 0; i < deckCount; i++){
        printf("%d", allDecksInfo[i].cardNumInThisDeck);
        if(allDecksInfo[i].cardNumInThisDeck == 0){
            printf("\n");
            continue;
        }else{
            printf(" ");
        }
        tracePointer = allDecks[i];
        for(j = 0; j < (allDecksInfo[i].cardNumInThisDeck - 1); j++){
            printf("%d ", tracePointer -> value);
            tracePointer = tracePointer -> next;
        }
        printf("%d\n", tracePointer -> value);
    }
}

int main(int argc, char ** argv){
    scanf("%d%d", &deckCount, &operationNum);
    allDecks = (struct card **) malloc(sizeof(struct card *) * deckCount);
    allDecksInfo = (struct  deckInfo *) malloc(sizeof(struct deckInfo) * deckCount);
    initAllDecksInfo();

    for(int i = 0; i < operationNum; i++){
        scanf("%d", &type);
        if(type == 1){
            scanf("%d%d", &targetDeckNumber, &addedNum);
            add();
        }else if(type == 2){
            scanf("%d", &targetDeckNumber);
            rm();
        }else if(type == 3){
            scanf("%d%d", &sourceDeckNumber, &destinationDeckNumber);
            move();
        }else if(type == 4){
            scanf("%d%d", &sourceDeckNumber, &destinationDeckNumber);
            merge();
        }
    }
    printResult();
    exit(0);
}
