#include <stdio.h>
#include <stdlib.h>


int N, B;
int swapCounter = 0;
int * inputArray;
int main(int argc, char ** argv){
    int i, j, min;
    scanf("%d%d", &N, &B);
    inputArray = (int *) malloc(sizeof(int) * N);
    for(i = 0; i < N; i++){
        scanf("%d", inputArray + i);
    }

    //selection sort
    for(i = 0; i < N - 1; i++){
        if(!(swapCounter ^ B)){
            break;
        }
        min = i;
        for(j = i + 1; j <= N - 1; j++){
            if(inputArray[j] < inputArray[min]){
                min = j;
            }
        }
        if(min != i){
            //swap
            inputArray[min] ^= inputArray[i];
            inputArray[i] ^= inputArray[min];
            inputArray[min] ^= inputArray[i];
            ++swapCounter;
        }
    }

    printf("The array is ");
    
    for(i = 0; i < N; i++){
        printf("%d ", inputArray[i]);
    }

    printf("after %d swaps.\n", swapCounter);


    exit(0);
}