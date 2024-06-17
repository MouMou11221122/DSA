#include <stdio.h>
#include <stdlib.h>



int arrSize;
int key;
int * arr;

int main(int argc, char ** argv)
{
    scanf("%d%d", &arrSize, &key);
    arr = malloc(sizeof(int) * arrSize);
    if(arr == NULL)
    {
        fprintf(stderr, "malloc error\n");
    }

    for(int i = 0; i < arrSize; i++)
    {
        scanf("%d", arr + i);
    }

    //binary search
    int left, right, mid;
    left = 0; right = arrSize - 1;

    while(left <= right)
    {
        printf("Searching %d in range [%d, %d].\n", key, left + 1, right + 1);
        mid = (left + right) >> 1;
        if(arr[mid] == key)
        {
            printf("Found at index %d.\n", mid + 1);
            exit(0);
        }else if(arr[mid] > key)
        {
            right = mid - 1;
        }else{
            left = mid + 1;
        }
    }
    
    printf("Searching %d in range [%d, %d].\n", key, left + 1, right + 1);        
    printf("Not found.\n");
    exit(0);
}