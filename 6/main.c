#include <stdio.h>
#include <stdlib.h>

int N, B;
int *inputArray;
int budgetCounter = 0;
int main(int argc, char **argv){
	int i, j, tmp;
	scanf("%d%d", &N, &B);
	inputArray = (int *) malloc(sizeof(int) *N);
	for(i = 0; i < N; i++){
		scanf("%d", inputArray + i);
	}
	
	//insertSort
	for(i = 1; i < N; i++){
		tmp = inputArray[i];
		for(j = i - 1; j > -1 && tmp < inputArray[j]; j--){
			if(budgetCounter != B){
				inputArray[j + 1] = inputArray[j];
				budgetCounter++;
			}else{
				break;
			}
		}
		inputArray[j + 1] = tmp;
		if(budgetCounter == B){
			break;
		}
	}

	printf("The array is ");
	for(i = 0; i < N; i++){
		printf("%d ", inputArray[i]);
	}
	printf("after %d back-moves.\n", budgetCounter);
	return 0;
}
