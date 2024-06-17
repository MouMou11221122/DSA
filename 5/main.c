
/*
 *	The following are constrains:
 *	1 <= classmate count <= 10 ^ 6
 *	1 <= incident count <= 5 * 10 ^ 5
 *	1 <= M <= 5 * 10 ^ 5
 *	0 <= PN <= PN-1 <= ... <= P1 <= 10 ^ 9
 *	1 <= ai, bi <= N
 *	0 <= qi <= 10^18
 *	1 <= mi <= M
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define int long long


#define INCIDENT1_ATTACK    1
#define INCIDENT2_REWARD    2
#define INCIDENT3_QUERY     3
#define INCIDENT4_QUERY     4


struct power{
    long long power;
    int indexToLabelArray;
};

struct label
{
    int indexToPowerArray;
    int previousAttackCount;
    int previousAttckPrefixArrayLimit;
    long long * previousAttckPrefixArray;
};

int incidentCount;
int classmateCount;
int M;
struct power * powerArray;
struct label * labelArray;
int * lastUpdateTime;
int rewardCounter;
int incidentType;
int operand1, operand2;
long long operand3;
long long oldAttackerPower;

int attackerlabelIndex, attackerRankIndex;
int victimLabelIndex, victimRankIndex;
int cleanObjectIndex;

/*
 *      only require expanding
 */
void dynamicTableResize()
{   
    labelArray[attackerlabelIndex].previousAttckPrefixArrayLimit = labelArray[attackerlabelIndex].previousAttckPrefixArrayLimit << 1;
    labelArray[attackerlabelIndex].previousAttckPrefixArray = realloc(labelArray[attackerlabelIndex].previousAttckPrefixArray, sizeof(long long) * labelArray[attackerlabelIndex].previousAttckPrefixArrayLimit);
    if(labelArray[attackerlabelIndex].previousAttckPrefixArray == NULL)
    {
        fprintf(stderr, "Memory Allocation Fail\n");
        exit(1);
    }
}

void attack()
{
    attackerlabelIndex = operand1 - 1;
    attackerRankIndex = labelArray[attackerlabelIndex].indexToPowerArray;
    if(attackerRankIndex > 0)
    {
        victimRankIndex = attackerRankIndex - 1;
        victimLabelIndex = powerArray[victimRankIndex].indexToLabelArray;
        /*
         *      clean up the victim's current power
         */
        powerArray[victimRankIndex].power += (rewardCounter - lastUpdateTime[victimLabelIndex]) * (classmateCount - victimRankIndex - 1);

        /*
         *      record the old power of attacker
         */
        oldAttackerPower = powerArray[attackerRankIndex].power + (rewardCounter - lastUpdateTime[attackerlabelIndex]) * (classmateCount - attackerRankIndex - 1);
        
        /*
         *      attacker's power becomes victims' power
         */
        powerArray[attackerRankIndex].power = powerArray[victimRankIndex].power;

        /*
         *      exchange attacker's and victim's power
         */
        powerArray[attackerRankIndex].indexToLabelArray = victimLabelIndex;
        labelArray[victimLabelIndex].indexToPowerArray = attackerRankIndex;
        powerArray[victimRankIndex].indexToLabelArray = attackerlabelIndex;
        labelArray[attackerlabelIndex].indexToPowerArray = victimRankIndex; 

        lastUpdateTime[attackerlabelIndex] = lastUpdateTime[victimLabelIndex] = rewardCounter;

        if(labelArray[attackerlabelIndex].previousAttackCount == 0)
        {
            labelArray[attackerlabelIndex].previousAttckPrefixArrayLimit = 3;
            labelArray[attackerlabelIndex].previousAttckPrefixArray = malloc(sizeof(long long) * 3);
            if(labelArray[attackerlabelIndex].previousAttckPrefixArray == NULL)
            {
                fprintf(stderr, "Memory Allocation Fail\n");
                exit(1);
            }         
            (labelArray[attackerlabelIndex].previousAttckPrefixArray)[labelArray[attackerlabelIndex].previousAttackCount] = \
                powerArray[attackerRankIndex].power - oldAttackerPower;
            labelArray[attackerlabelIndex].previousAttackCount++;
            return;
        }else if(labelArray[attackerlabelIndex].previousAttackCount == labelArray[attackerlabelIndex].previousAttckPrefixArrayLimit)
        {
            dynamicTableResize();        
        }

        /*
         *      keep track of the increase of the attack
         */
        (labelArray[attackerlabelIndex].previousAttckPrefixArray)[labelArray[attackerlabelIndex].previousAttackCount] = 
            (labelArray[attackerlabelIndex].previousAttckPrefixArray)[labelArray[attackerlabelIndex].previousAttackCount - 1] +  
            (powerArray[attackerRankIndex].power - oldAttackerPower);
 
        labelArray[attackerlabelIndex].previousAttackCount++;
    }
}


void reward()
{
    rewardCounter++;
}

void query3()
{
    int left, right, middle;

    /*
     *      do the binary search first
     */
    left = 0;
    right = classmateCount - 1;
    middle = (left + right) / 2;

    /*
     *      clean up
     */
    powerArray[0].power += (rewardCounter - lastUpdateTime[powerArray[0].indexToLabelArray]) \
            * (classmateCount - 1);
    lastUpdateTime[powerArray[0].indexToLabelArray] = rewardCounter;

    if(operand3 > powerArray[0].power)
    {
        printf("0 0\n");
        return;
    }

    while(right - left > 1)
    {
        middle = (left + right) / 2;

        /*
         *      clean up
         */
        powerArray[middle].power += (rewardCounter - lastUpdateTime[powerArray[middle].indexToLabelArray]) \
            * (classmateCount - middle - 1);
        lastUpdateTime[powerArray[middle].indexToLabelArray] = rewardCounter;

        if(powerArray[middle].power >= operand3)
        {
            left = middle;
        }else{
            right = middle - 1;
        }
    }

    if(left == right)
    {        
        /*
        powerArray[left].power += (rewardCounter - lastUpdateTime[powerArray[left].indexToLabelArray]) \
                * (classmateCount - 1);
        lastUpdateTime[powerArray[left].indexToLabelArray] = rewardCounter;
        */
        printf("%lld %lld\n", left + 1, powerArray[left].indexToLabelArray + 1);
    }else{  // right == left + 1
        powerArray[right].power += (rewardCounter - lastUpdateTime[powerArray[right].indexToLabelArray]) \
                * (classmateCount - right - 1);
        lastUpdateTime[powerArray[right].indexToLabelArray] = rewardCounter;
        if(operand3 <= powerArray[right].power)
        {
            printf("%lld %lld\n", right + 1, powerArray[right].indexToLabelArray + 1);
        }else{
            /*
            powerArray[left].power += (rewardCounter - lastUpdateTime[powerArray[left].indexToLabelArray]) \
                    * (classmateCount - 1);
            lastUpdateTime[powerArray[left].indexToLabelArray] = rewardCounter;
            */
            printf("%lld %lld\n", left + 1, powerArray[left].indexToLabelArray + 1);
        }
    }
}

void query4()
{
    long long increasedPower;
    int startIndex;
    long long base;

    if(labelArray[operand1 - 1].previousAttackCount == 0)
    {
        printf("0\n");
        return;
    }

    startIndex = labelArray[operand1 - 1].previousAttackCount - operand2 - 1;
    if(operand2 >= labelArray[operand1 - 1].previousAttackCount)
    {
        base = 0;
    }else{
        base = (labelArray[operand1 - 1].previousAttckPrefixArray)[startIndex];    
    }
    increasedPower = (labelArray[operand1 - 1].previousAttckPrefixArray)[labelArray[operand1 - 1].previousAttackCount - 1] - \
            base;
    printf("%lld\n", increasedPower);
}

int main(){
    int i, j;
    scanf("%lld%lld%lld", &classmateCount, &incidentCount, &M);
    powerArray = malloc(sizeof(struct power) * classmateCount);
    labelArray = malloc(sizeof(struct label) * classmateCount);
    lastUpdateTime = malloc(sizeof(int) * classmateCount);

    for(i = 0; i < classmateCount; i++)
    {
        lastUpdateTime[i] = 0;
        labelArray[i].indexToPowerArray = i;
        labelArray[i].previousAttackCount = 0;
        labelArray[i].previousAttckPrefixArray = NULL;
        scanf("%lld", &(powerArray[i].power));
        powerArray[i].indexToLabelArray = i;
    }

    for(i = 0; i < incidentCount; i++)
    {
        scanf("%lld", &incidentType);
        if(incidentType == INCIDENT1_ATTACK)
        {
            scanf("%lld", &operand1);
            attack();
        }else if(incidentType == INCIDENT2_REWARD)
        {
            reward();
        }else if(incidentType == INCIDENT3_QUERY)
        {
            scanf("%lld", &operand3);
            query3();
        }else if(incidentType == INCIDENT4_QUERY)
        {
            scanf("%lld%lld", &operand1, &operand2);
            query4();
        }
    }

    /*
     *      the second part
     **/
    printf("\n");
    for(i = 0; i < classmateCount; i++)
    {
        printf("%lld", labelArray[i].previousAttackCount);
        if(labelArray[i].previousAttackCount == 0)
        {
            printf("\n");
        }else{
            printf(" %lld", (labelArray[i].previousAttckPrefixArray)[0]);
            for(j = 1; j <= labelArray[i].previousAttackCount - 1; j++)
            {
                printf(" %lld", (labelArray[i].previousAttckPrefixArray)[j] - (labelArray[i].previousAttckPrefixArray)[j - 1]);
            }
            printf("\n");
        }
    }
    exit(0);
}
