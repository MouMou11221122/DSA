#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define MAX_CHARACTER_SIZE 5000000
#define FOUND_SHIFT 1
#define NOT_FOUND_SHIFT 0

char text[MAX_CHARACTER_SIZE + 1];
char pattern[MAX_CHARACTER_SIZE + 1];
long long failure_function[MAX_CHARACTER_SIZE];
long long patternSize, textSize;
long long suffix_equal_prefix_count;
long long sepArray[MAX_CHARACTER_SIZE];
long long min[MAX_CHARACTER_SIZE];

/*
 *		kmp info
 */
long long kmpMatch[MAX_CHARACTER_SIZE];
char kmpShift[MAX_CHARACTER_SIZE];
long long matchNum;
long long accumulatedArray[MAX_CHARACTER_SIZE];
long long min_index = -1, min_value = -1;
long long matchTable[MAX_CHARACTER_SIZE];

long long oldPatternSize;



void failure()
{
	long long try_to_match_index_i = 0;
	failure_function[0] = -1;
	for(long long i = 1; i < patternSize; i++)
	{
		if(pattern[try_to_match_index_i] == pattern[i])
		{
			failure_function[i] = try_to_match_index_i;			//the value of the array indicates having been matched to
			try_to_match_index_i++;
		}else{
			while(try_to_match_index_i > 0)
			{
				try_to_match_index_i = failure_function[try_to_match_index_i - 1] + 1;
				if(pattern[try_to_match_index_i] == pattern[i])
					break;
			}
			if(pattern[try_to_match_index_i] == pattern[i])
			{
					failure_function[i] = try_to_match_index_i;
					try_to_match_index_i++;
			}else{
					failure_function[i] = -1;
			}
		}
	}
}



/*
 *		count the suffix equal prefix array
 */
void count_sepArray()
{
	long long trace;
	sepArray[0] = 1;
	for(long long i = 1; i < patternSize; i++)
	{
		trace = failure_function[i];
		while(trace >= 0)
		{
			sepArray[i]++;
			trace = failure_function[trace];
		}
		sepArray[i]++;
	}
}


/*
 *	simultaneously find the failure function and min function by finded failure function
 */
void optimal_count_sepArray()
{
	long long try_to_match_index_i = 0;
	failure_function[0] = -1;
	for(int i = 1; i < patternSize; i++)
	{
		while(try_to_match_index_i > -1)
		{
			while(i < patternSize && pattern[i] == pattern[try_to_match_index_i])
			{
				failure_function[i] = try_to_match_index_i;
				sepArray[i] = 1 + sepArray[failure_function[i]];
				///TODO:construct the min
				if(min_value == -1)
				{
						min_value = try_to_match_index_i;
						min_index = i;
				}
				i++;
				try_to_match_index_i++;
			}
			if(i < patternSize)
			{
				if(try_to_match_index_i == 0)
				{
					failure_function[i] = -1;
					sepArray[i] = 0;
					break;
				}else{
					try_to_match_index_i = failure_function[try_to_match_index_i - 1] + 1;
				}
			}else{
				break;
			}
		}
	}

	if(min_index == -1)
			min_value = 0;
	
}



/*
 *		construct matchTable[0 ~ patternSize - 1]
 */
void processing_matchTable()
{
	long long expect_to_match_ith;
	long long original_i;
	long long original_i_matching_boundary;
	for(int i = 1; i < patternSize; i++)
	{
		if(pattern[i] != pattern[0])
		{
			matchTable[i] = 0;
		}else{
			expect_to_match_ith = 0;
			/*find the maximum matching*/
			while(i + expect_to_match_ith < patternSize && pattern[i + expect_to_match_ith] == pattern[expect_to_match_ith])
			{
				expect_to_match_ith++;
			}
			expect_to_match_ith--;
			matchTable[i] = expect_to_match_ith + 1;
						
			/*	start the machine	*/
			while(expect_to_match_ith > -1)
			{
				/*	initiate the information of the start and end boundary	*/
				original_i = i;
				original_i_matching_boundary = i + expect_to_match_ith;

				expect_to_match_ith = failure_function[expect_to_match_ith];
				for(i++; i < original_i_matching_boundary - expect_to_match_ith; i++)
				{
					matchTable[i] = matchTable[i - original_i];
				}
				if(i >= patternSize)	break;
				for(expect_to_match_ith++; i + expect_to_match_ith < patternSize; expect_to_match_ith++)
				{
					if(pattern[expect_to_match_ith] != pattern[i + expect_to_match_ith])
					{
						break;
					}
				}
				expect_to_match_ith--;
				matchTable[i] = expect_to_match_ith + 1;
			}
		}
	}
	matchTable[0] = patternSize;
}




/*
 *		leverage the failure function
 *		an extended kmp algorithm
 */
void extended_kmp()
{
	long long subpatternSize;
	long long expect_to_match_ith;

	///initialize kmpMatch[i] and kmpShift for i = 0
	char tmp = pattern[0];
	for(long long k = 0; k < textSize; k++)
	{
		if(tmp == text[k])
		{
			kmpShift[k] = FOUND_SHIFT;
			matchNum++;
		}
	}
	kmpMatch[0] = matchNum;
	///

	for(long long i = 1; i < patternSize; i++)
	{
		subpatternSize = i + 1;
		matchNum = 0;

		//run the primary kmp
		for(long long j = 0; j <= textSize - subpatternSize; j++)
		{
			if(kmpShift[j] == NOT_FOUND_SHIFT)
			{
				continue;
			}
			expect_to_match_ith = 0;
			while(expect_to_match_ith >= 0)
			{
				while(pattern[expect_to_match_ith] == text[j + expect_to_match_ith])
				{
					if(expect_to_match_ith == subpatternSize - 1)
					{
						matchNum++;
						//fprintf( stdout, "find %dth matching for the subpattern : pattern[0 ~ %d] in shift %d of the text\n", matchNum - 1, i, j);
						if(failure_function[expect_to_match_ith] == -1)
						{
							j = j + expect_to_match_ith + 1;
							expect_to_match_ith = 0;
							continue;
						}
						j = j + expect_to_match_ith - failure_function[expect_to_match_ith];
						expect_to_match_ith = failure_function[expect_to_match_ith] + 1;
					}else{
							expect_to_match_ith++;
					}
				}
				if(expect_to_match_ith == 0)
				{
					expect_to_match_ith = -1;
				}else{
					j = j + expect_to_match_ith - (failure_function[expect_to_match_ith - 1] + 1);
					expect_to_match_ith = failure_function[expect_to_match_ith - 1] + 1;
				}
			}
			//doesn't match and do something with kmpShift array
			kmpShift[j] = NOT_FOUND_SHIFT;
		}
		kmpMatch[i] = matchNum;
	}
}

void optimal_kmp()
{
	long long expect_to_match_ith;	//0 ~ patternSize - 1
	long long limit;
	long long original;
	bool first;
	long long predication;
	long long min;
	long long exceed;
	for(long long i = 0; i < textSize; i++)
	{
		if(i > textSize - patternSize)
		{
			limit = i - (textSize - patternSize);
		}else{
			limit = 0;
		}
																		
		expect_to_match_ith = 0;

		while(expect_to_match_ith >= 0)
		{
			first = true;
			while(expect_to_match_ith < patternSize - limit && pattern[expect_to_match_ith] == text[i + expect_to_match_ith])
			{				
				if(first == true)
				{
					predication = 0;
					while(predication < patternSize - limit && pattern[predication] == text[i + predication])
					{
						predication++;
					}		
					predication--;
					first = false;
				}
				if(matchTable[expect_to_match_ith] > 0)
				{
					if(expect_to_match_ith + matchTable[expect_to_match_ith] - 1 < predication)
					{
						min = matchTable[expect_to_match_ith] - 1;
					}else{
						exceed =  predication + 1 - expect_to_match_ith;
						while(i + expect_to_match_ith + exceed < textSize &&  exceed < patternSize && text[i + expect_to_match_ith + exceed] == pattern[exceed])
						{
							exceed++;
						} 
						exceed--;
						min = exceed;
					}
					accumulatedArray[min]++;
				}
				expect_to_match_ith++;
			}

			if(i == textSize - 1)
			{
				return;
			}

			if(expect_to_match_ith == patternSize - limit)
			{
				expect_to_match_ith--;			
				original = i;
				i = i + expect_to_match_ith + 1;
				if(i > textSize - patternSize)
					limit = limit + expect_to_match_ith + 1;
				if(original < textSize - patternSize && i > textSize - patternSize)
					limit = limit - (textSize - patternSize - original);
				if(i >= textSize)
						return;
				expect_to_match_ith = 0;
			}else if(expect_to_match_ith == 0){
				expect_to_match_ith = -1;
			}else{
				original = i;
				i = i + expect_to_match_ith;
				if(i > textSize - patternSize)
					limit = limit + expect_to_match_ith;
				if(original < textSize - patternSize && i > textSize - patternSize)
					limit = limit - (textSize - patternSize - original);				
				if(i >= textSize)
						return;
				expect_to_match_ith = 0;
			}		
		}
	}
}

void optimal_kmp2()
{
	long long expect_to_match_ith;	//0 ~ patternSize - 1
	long long old_i;
	long long original_i_matching_boundary;
	for(long long i = 0; i < textSize; i++)
	{
		for(expect_to_match_ith = 0; expect_to_match_ith < patternSize &&  i + expect_to_match_ith < textSize; expect_to_match_ith++)
		{
			if(pattern[expect_to_match_ith] != text[i + expect_to_match_ith])	break;
		}
		expect_to_match_ith--;

		while(expect_to_match_ith > -1){
			old_i = i;
			original_i_matching_boundary = i + expect_to_match_ith;
			expect_to_match_ith = failure_function[expect_to_match_ith];
			for(; i < original_i_matching_boundary - expect_to_match_ith; i++)
			{
				if(i == old_i)
				{
					accumulatedArray[original_i_matching_boundary - i]++;
					continue;
				}
				if(matchTable[i - old_i] > 0)	accumulatedArray[matchTable[i - old_i] - 1]++;
			}
			if(i == textSize)	return;	
			/*	i == original_i_matching_boundary - expect_to_match_ith	*/
			for(expect_to_match_ith++; expect_to_match_ith < patternSize &&  i + expect_to_match_ith < textSize; expect_to_match_ith++)
			{
				if(pattern[expect_to_match_ith] != text[i + expect_to_match_ith])	break;
			}
			expect_to_match_ith--;
		}
	}
}

void test_kmp()
{
	failure();
	extended_kmp();
}

void multiplication()
{
	/*
	if(oldPatternSize > textSize)
	{
		patternSize = oldPatternSize;
	}
	*/
	for(long long i = 0; i < patternSize; i++)
	{
		printf("%lld\n", (sepArray[i]+1) * kmpMatch[i]);
	}
}

void show_the_view()
{
	for(long long i = 0; i < textSize; i++)
	{
		printf("%c ", text[i]);
	}
	printf("\n");

	for(long long i = 0; i < patternSize; i++)
	{
		printf("%c ", pattern[i]);
	}
	printf("\n");
}

void test_matchTable()
{
	for(int i = 0; i < patternSize; i++)
	{
		printf("%lld ", failure_function[i]);
	}
	printf("\n");

	for(int i = 0; i < patternSize; i++)
	{
		printf("%lld ", matchTable[i]);
	}
	printf("\n");
}

/*
 *		constrain : 0 < |S| <= 5 * 10 ^ 6, 0 < |C| <= 5 * 10 ^ 6
 *		Σ = {a, b, c, ..., y, z}
 */
int main(int argc, char ** argv)
{
		scanf("%s%s", text, pattern);
		patternSize = strlen(pattern);
		textSize = strlen(text);
		/*
		if(textSize < patternSize)
		{
			oldPatternSize = patternSize;
			patternSize = textSize;
		}
		*/

		optimal_count_sepArray();
		processing_matchTable();
		//test_matchTable();
		
		
		optimal_kmp2();

		kmpMatch[patternSize - 1] = accumulatedArray[patternSize - 1];
		for(int i = patternSize - 2; i >= 0; i--)
		{
			kmpMatch[i] = kmpMatch[i + 1] + accumulatedArray[i];
		}
		
		
		//for(int i = 0; i < patternSize; i++) printf("%lld ", kmpMatch[i]);
		//printf("\n");
		
		multiplication();
		
		exit(0);
}
