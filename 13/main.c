#include<stdio.h>
#include<stdlib.h>

int Budget;
int BudgetCopy;

void swap(int* input, int i, int j)
{
	int tmp;
	tmp = input[i];
	input[i] = input[j];
	input[j] = tmp;
	--Budget;
}

int partition(int* input, int left, int right)
{
	int pivot = left;
	int i = left + 1, j = right;
	while(i < j)
	{
		while(i <= right && input[i] <= input[pivot]) i++;
		while(j > left && input[j] > input[pivot])	j--;
		if(i >= j)	break;
		swap(input, i, j);
		if(Budget == 0)	return -1;
	}
	if(j > left && input[j] < input[pivot])
	{
		swap(input, j, pivot);
		if(Budget == 0)	return -1;
	}
	return j;
}


void qqsort(int* input, int left, int right)
{
	if(left < right)
	{
		int idx;
		idx = partition(input, left, right);
		if(Budget == 0)	return;	
		qqsort(input, left, idx - 1);
		if(Budget == 0)	return;
		qqsort(input, idx + 1, right);
	}
}

void result(int* input, int N)
{
	printf("The array is ");
	for(int i = 0; i < N; i++)
	{
		printf("%d ", input[i]);
	}
	printf("after %d swaps.\n", BudgetCopy - Budget);

}

int main(int argc, char** argv)
{
		int N;
		int *input;
		scanf("%d%d", &N, &Budget);
		input = malloc(sizeof(int) * N);
		BudgetCopy = Budget;
		for(int i = 0; i < N; i++)
		{
			scanf("%d", &input[i]);
		}
		qqsort(input, 0, N -1);
		result(input, N);
		exit(0);
}
