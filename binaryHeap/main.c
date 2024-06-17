#include <stdio.h>
#include <stdlib.h>



int main(int argc, char ** argv)
{   
    int N;
    int * heap = NULL;
    int heapSize = 0;
    //int key;
    int tmpData, tmpIndex;
    int i;
    scanf("%d", &N);
    heap = malloc(sizeof(int) * N);

    heapSize++;
    scanf("%d", heap + heapSize - 1);

    for(int i = 1; i < N; i++)
    {
        heapSize++;
        scanf("%d", heap + heapSize - 1);

        tmpIndex = heapSize - 1;
        tmpData = heap[tmpIndex];

        while(tmpIndex > 0 && tmpData > heap[(tmpIndex - 1) >> 1])
        {
            heap[tmpIndex] = heap[(tmpIndex - 1) >> 1];
            tmpIndex = (tmpIndex - 1) >> 1;
        }
        heap[tmpIndex] = tmpData;
    }

    for(i = 0; i <= N - 2; i++){
        printf("%d ", heap[i]);
    }
    printf("%d\n", heap[i]);

    exit(0);
}