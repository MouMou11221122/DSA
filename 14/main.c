#include<stdio.h>
#include<stdlib.h>

#define FETCH_DATA(i, j, arr) 											\
    ((arr == 'T') ? (*(text + (i) * (textSize + 1) + (j))) : 			\
    ((arr == 'P') ? (*(pattern + (i) * (patternSize + 1) + (j))) : 0))
#define CONVERT_CHAR(c) ((c > 96) ? (c - 71) : (c - 65))
int rowSize, textSize, patternSize; 
char * text, * pattern;
long long * textHash, * patternHash;
long long * failureFucntion;
long long modulo;

int validShfit[1000000];
int validShiftCount;
int noMatch[1000000];
int noMatchCount;

void processInput()
{
    scanf("%d%d%d%lld", &rowSize, &textSize, &patternSize, &modulo);
    text = malloc(sizeof(char) * rowSize * (textSize + 1));
    pattern = malloc(sizeof(char) * rowSize * (patternSize + 1));
    textHash = malloc(sizeof(long long) * textSize);
    patternHash = malloc(sizeof(long long) * patternSize);
    failureFucntion = malloc(sizeof(long long) * patternSize);

    for(int i = 0; i < rowSize; i++)
    {
        scanf("%s", text + i * (textSize + 1));
    }
    for(int i = 0; i < rowSize; i++)
    {
        scanf("%s", pattern + i * (patternSize + 1));
    }
}

/*
void countHashValue()
{
    long long * modulo_of_power = malloc(sizeof(long long) * textSize);
    ///  count hash array of the text   ///
    for(int j = 0; j < textSize; j++)
    {
        modulo_of_power[j] = 1;
        textHash[j] = CONVERT_CHAR(*(text + (rowSize - 1) * (textSize + 1) + j)) % modulo;
    }
    for(int i = rowSize - 2; i >= 0; i--)
    {
        for(int j = 0; j < textSize; j++)
        {
            modulo_of_power[j] = (modulo_of_power[j] * (52 % modulo)) % modulo;
            textHash[j] = ((((CONVERT_CHAR(*(text + i * (textSize + 1) + j)) % modulo) * modulo_of_power[j]) % modulo) + textHash[j]) % modulo;
        }
    }

    ///  count hash array of the pattern  ///
    for(int j = 0; j < patternSize; j++)
    {
        modulo_of_power[j] = 1;
        patternHash[j] = CONVERT_CHAR(*(pattern + (rowSize - 1) * (patternSize + 1) + j)) % modulo;
    }
    for(int i = rowSize - 2; i >= 0; i--)
    {
        for(int j = 0; j < patternSize; j++)
        {
            modulo_of_power[j] = (modulo_of_power[j] * (52 % modulo)) % modulo;
            patternHash[j] = ((((CONVERT_CHAR(*(pattern + i * (patternSize + 1) + j)) % modulo) * modulo_of_power[j]) % modulo) + patternHash[j]) % modulo;
        }
    }
    free(modulo_of_power);
}
*/

void countHashValue()
{
    /*  count hash array of the text   */
    for(int j = 0; j < textSize; j++)
    {
        textHash[j] = CONVERT_CHAR(*(text + 0 * (textSize + 1) + j)) % modulo;
    }
    for(int i = 1; i < rowSize; i++)
    {
        for(int j = 0; j < textSize; j++)
        {
			textHash[j] = (((textHash[j] * (52 % modulo)) % modulo) + (CONVERT_CHAR(*(text + i * (textSize + 1) + j)) % modulo)) % modulo;
        }
    }

    /*  count hash array of the pattern  */
    for(int j = 0; j < patternSize; j++)
    {
        patternHash[j] = CONVERT_CHAR(*(pattern + 0 * (patternSize + 1) + j)) % modulo;
    }
    for(int i = 1; i < rowSize; i++)
    {
        for(int j = 0; j < patternSize; j++)
        {
			patternHash[j] = (((patternHash[j] * (52 % modulo)) % modulo) + (CONVERT_CHAR(*(pattern + i * (patternSize + 1) + j)) % modulo)) % modulo;
        }
    }
}

void failure()
{
    failureFucntion[0] = -1;
    int expect_to_match_ith;
    for(int i = 1; i < patternSize; i++)
    {
        expect_to_match_ith = 0;
        while(expect_to_match_ith > -1)
        {
            for(; i < patternSize; expect_to_match_ith++, i++)
            {
                if(patternHash[i] != patternHash[expect_to_match_ith])
                    break;
                else
                    failureFucntion[i] = expect_to_match_ith;
            }
            if(i == patternSize)    return;
			if(expect_to_match_ith == 0)
			{
				failureFucntion[i] = -1;
				i++;
				break;
			}
            expect_to_match_ith = failureFucntion[expect_to_match_ith - 1] + 1;
        }
		i--;
    }
}


void check(int shift)
{
    validShfit[validShiftCount] = shift;
    validShiftCount++;
    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < patternSize; j++)
        {
            if(*(pattern + i * (patternSize + 1) + j) != *(text + i * (textSize + 1) + shift + j))
            {
                noMatch[noMatchCount] = shift;
                noMatchCount++;
                return;
            }
        }
    }
}

void kmpMatcher()
{
    failure();
    int expect_to_match_ith;
    for(int i = 0; i <= textSize - patternSize; i++)
    {
        expect_to_match_ith = 0;
        while(expect_to_match_ith > -1)
        {
            for(; expect_to_match_ith < patternSize && i <= textSize - patternSize; expect_to_match_ith++)
            {
                if(patternHash[expect_to_match_ith] != textHash[i + expect_to_match_ith])   break;
            }
            if(expect_to_match_ith == 0)
			{
				i++;
				break;
			}
            if(i > textSize - patternSize)  return;
            else if(expect_to_match_ith == patternSize)     check(i);
            i = i + expect_to_match_ith - (failureFucntion[expect_to_match_ith - 1] + 1);
            expect_to_match_ith = failureFucntion[expect_to_match_ith - 1] + 1;           
        }
        i--;
    }   
}


void printResult()
{
    int i;
    for(i = 0; i < textSize - 1; i++)
    {
       printf("%lld ", textHash[i]);
    }
    printf("%lld\n", textHash[i]);

    for(i = 0; i < patternSize - 1; i++)
    {
       printf("%lld ", patternHash[i]);
    }
    printf("%lld\n", patternHash[i]);

	if(validShiftCount == 0)
	{
		printf("-1\n");
	}else{
    	for(i = 0; i < validShiftCount - 1; i++)
   		{
       		printf("%d ", validShfit[i]);
    	}
    	printf("%d\n", validShfit[i]);
	}

	if(noMatchCount == 0)
	{
		printf("-1\n");
	}else{
    	for(i = 0; i < noMatchCount - 1; i++)
    	{
       		printf("%d ", noMatch[i]);
    	}
    	printf("%d\n", noMatch[i]);
	}
}

int main(int argc, char ** argv)
{
    processInput();
    countHashValue();
    kmpMatcher();
    printResult();
    exit(0);
}
