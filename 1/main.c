#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
//#define BUFFERSIZE (1 << 20)


int N;
int counter = 0;
int final_counter = 0;
int * inputArray;
int * resultArray;
int resultArrayPointer = 0;
int ** addrArray = NULL;
bool * inputArrayBitmap;
int * processingArray;
int * state;                    //1 for < , 0 for == , -1 for >
int * selectIndex;
int MAX_PER_NUM;

bool * finalBitmap;




void simpleSort(){
	//simple sort for inputArray
	int i, j;
	for(i = 1; i <= N - 1; i++){
		int tmp = inputArray[i];
		for(j = i; j > 0 && inputArray[j - 1] > tmp; j--){
			inputArray[j] = inputArray[j - 1];			
		}
		inputArray[j] = tmp;
	}	
}


void addToResult(){
    for(int i = 0; i < N; i++){
        (resultArray + counter * N)[i] = processingArray[i];
    }
    counter++;
}

void printResult(){
    int i, j;
    printf("%d\n", counter);
    for(i = 0; i < counter; i++){
        for(j = 0; j < N - 1; j++){
            printf("%d ", (resultArray + i * N)[j]);
        }
        printf("%d\n", (resultArray + i * N)[j]);
    }
}


/*
void printResult(){
    int tmpCount = final_counter;
    int currentPointer = 0;
    int i;
    printf("%d\n",tmpCount);
    if(tmpCount == 0){
        return;
    }
    while(tmpCount > 1){
        while(finalBitmap[currentPointer] == false){    
            currentPointer++;
        }
        for(i = 0; i < N - 1; i++){
            printf("%d ", (resultArray + N * currentPointer)[i]);
        }
        printf("%d\n", (resultArray + N * currentPointer)[i]);
        tmpCount--;
        currentPointer++;
    }
    while(finalBitmap[currentPointer] == false){
        currentPointer++;
    }
    for(int i = 0; i < N - 1; i++){
        printf("%d ", (resultArray + N * currentPointer)[i]);
    }
    printf("%d\n", (resultArray + N * currentPointer)[i]);
}
*/
/*
void printResult(){
    int tmpCount = final_counter;
    int currentPointer = 0;
    int i;
    printf("%d\n",tmpCount);
    if(tmpCount == 0){
        return;
    }
    while(tmpCount > 1){
        while(finalBitmap[currentPointer] == false){    
            currentPointer++;
        }
        for(i = 0; i < N - 1; i++){
            printf("%d ", (addrArray[currentPointer])[i]);
        }
        printf("%d\n", (addrArray[currentPointer])[i]);
        tmpCount--;
        currentPointer++;
    }
    while(finalBitmap[currentPointer] == false){
        currentPointer++;
    }
    for(int i = 0; i < N - 1; i++){
        printf("%d ", (addrArray[currentPointer])[i]);
    }
    printf("%d\n", (addrArray[currentPointer])[i]);
}
*/

/*
void adjustResult(){
    finalBitmap[0] = true;
    bool theSame = true;
    if(counter == 0){
        final_counter = 0;
        return;
    }
    final_counter++;
    for(int i = 1; i < counter; i++){
        theSame = true;
        for(int j = 0; j < N; j++){
            if((resultArray + N * i)[j] != (resultArray + N * (i - 1))[j]){
                theSame = false;
                final_counter++;
                break;
            }
        }
        if(!theSame){
            finalBitmap[i] = true;
        }else{
            finalBitmap[i] = false;
        }
    }
}
*/
/*
void adjustResult(){
    finalBitmap[0] = true;
    bool theSame = true;
    if(counter == 0){
        final_counter = 0;
        return;
    }
    final_counter++;
    for(int i = 1; i < counter; i++){
        theSame = true;
        for(int j = 0; j < N; j++){
            if((addrArray[i])[j] != (addrArray[i - 1])[j]){
                theSame = false;
                final_counter++;
                break;
            }
        }
        if(!theSame){
            finalBitmap[i] = true;
        }else{
            finalBitmap[i] = false;
        }
    }
}
*/
/*
void pp(){
    int i, j;
    for(i = 0; i < counter; i++){
        for(j = 0; j < N - 1; j++){
            printf("%d ", (addrArray[i])[j]);
        }
        printf("%d\n", (addrArray[i])[j]);
    }
}
*/
/*
void sortResult(){
    int i, j, k;
    int * tmpAddr;
    int tmpData;
    //initialization
    for(int i = 0; i < counter; i++){
        addrArray[i] = resultArray + i * N;
    }
    //start sorting
    for(i = N - 1; i >= 0; i--){
        for(j = 1; j < counter; j++){
            tmpAddr = addrArray[j]; 
            tmpData = (addrArray[j])[i];
            for(k = j; k > 0; k--){
                if(tmpData < (addrArray[k - 1])[i]){
                    addrArray[k] = addrArray[k - 1];
                }else{
                    break;
                }
            }
            addrArray[k] = tmpAddr;
        }
    }
}
*/

/*
void addToResult(){
    for(int i = 0; i < N; i++){
        (resultArray + resultArrayPointer * N)[i] = processingArray[i];
    }
    resultArrayPointer++;
}
*/

void permutation(int index){
    if(index == N - 1){
        if(state[index - 1] == 1){
            for(int i = 0; i < selectIndex[index - 1]; i++){
                if(!inputArrayBitmap[i] && (inputArray[i] < processingArray[index - 1])){
                    inputArrayBitmap[i] = true;
                    processingArray[index] = inputArray[i];
                    selectIndex[index] = i;
                    //counter++;
                    addToResult();
                    inputArrayBitmap[i] = false;
                    return;
                }
            }
        }else if(state[index - 1] == -1){
            for(int i = (selectIndex[index - 1] + 1); i < N; i++){
                if(!inputArrayBitmap[i] && (inputArray[i] > processingArray[index - 1])){
                    inputArrayBitmap[i] = true;
                    processingArray[index] = inputArray[i];
                    selectIndex[index] = i;
                    //counter++;
                    addToResult();
                    inputArrayBitmap[i] = false;
                    return;
                }
            }
        }
    }else if(index == 0){
        for(int i = 0; i < N; i++){
            inputArrayBitmap[i] = true;
            processingArray[index] = inputArray[i];
            selectIndex[index] = i;
            permutation(index + 1);
            inputArrayBitmap[i] = false;
            while((i < N - 1) && inputArray[i] == inputArray[i + 1]){
                i++;
            }
        }
    }else if(index == 1){
        for(int i = 0; i < N; i++){
            if(inputArrayBitmap[i] == false){
                inputArrayBitmap[i] = true;
                processingArray[index] = inputArray[i];
                selectIndex[index] = i;
                if(processingArray[index] > processingArray[index - 1]){
                    state[index] = 1;
                }else if(processingArray[index] < processingArray[index - 1]){
                    state[index] = -1;
                }else{
                    state[index] = 0;
                    inputArrayBitmap[i] = false;
                    continue;    
                }
                permutation(index + 1);
                inputArrayBitmap[i] = false;
                while((i < N - 1) && inputArray[i] == inputArray[i + 1]){
                    i++;
                }
            }
        }
    }else{  //index >= 2 and index < N - 1        
        if(state[index - 1] == 1){
            for(int i = 0; i < selectIndex[index - 1]; i++){
                if(!inputArrayBitmap[i] && (inputArray[i] < processingArray[index - 1])){
                    inputArrayBitmap[i] = true;
                    processingArray[index] = inputArray[i];
                    selectIndex[index] = i;
                    state[index] = -1;
                    permutation(index + 1);
                    inputArrayBitmap[i] = false;
                    while((i < N - 1) && inputArray[i] == inputArray[i + 1]){
                        i++;
                    }
                }
            }
        }else if(state[index - 1] == -1){
            for(int i = (selectIndex[index - 1] + 1); i < N; i++){
                if(!inputArrayBitmap[i] && (inputArray[i] > processingArray[index - 1])){
                    inputArrayBitmap[i] = true;
                    processingArray[index] = inputArray[i];
                    selectIndex[index] = i;
                    state[index] = 1;
                    permutation(index + 1);
                    inputArrayBitmap[i] = false;
                    while((i < N - 1) && inputArray[i] == inputArray[i + 1]){
                        i++;
                    }
                }
            }
        }
    }

}

int main(int argc, char ** argv){
    MAX_PER_NUM = 2 * ((int)(pow(10, 5)));
    scanf("%d", &N);
    inputArray = (int *) malloc(sizeof(int) * N);
    inputArrayBitmap = (bool *) malloc(sizeof(bool) * N);
    processingArray = (int *) malloc(sizeof(int) * N);
    resultArray = (int *) malloc(sizeof(int) * MAX_PER_NUM * N);
    state = (int *) malloc(sizeof(int) * N);
    selectIndex = (int *) malloc(sizeof(int) * N);
    //finalBitmap = (bool *) malloc(sizeof(bool) * BUFFERSIZE);
    
    /*
    for(int i = 0; i < BUFFERSIZE; i++){
        finalBitmap[i] = false;
    }
    */
    for(int i = 0; i < N; i++){
        scanf("%d", inputArray + i);
    }


    if(N == 1){
        printf("1\n%d\n", *inputArray);
        return 0;
    }else if(N == 2){
        //int larger, smaller;
        if(inputArray[0] == inputArray[1]){
            printf("1\n%d %d\n", inputArray[0], inputArray[0]);
        }else if(inputArray[0] < inputArray[1]){
            printf("2\n%d %d\n%d %d\n", inputArray[0], inputArray[1], inputArray[1], inputArray[0]);
        }else{
            printf("2\n%d %d\n%d %d\n", inputArray[1], inputArray[0], inputArray[0], inputArray[1]);
        }
        return 0;
    }


    simpleSort();
    
    //initialize inputArrayBitmap
    for(int i = 0; i < N; i++){
        inputArrayBitmap[i] = false;
    }
    
    permutation(0); 
    //addrArray = (int **) malloc(sizeof(int *) * counter); 
    //sortResult();
    //pp(); 
    //adjustResult();
    printResult();
    exit(0);
}