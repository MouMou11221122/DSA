#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIORITY_ADD        1
#define PRIORITY_SUB        1
#define PRIORITY_MULT       2
#define PRIORITY_DIV        2
#define PRIORITY_MOD        2

char buf1[4001];
//char buf2[4001];
int Index = 0;
int charCount = 0;
long long stack[2001];
int stackTop;

char st[4001];
int stTop;


char stPeep()
{
    return st[stTop];
}

bool stEmpty()
{
    if(stTop == -1) return true;
    return false;
    
}

bool priorityLargeThan(char tokenA)
{
    char tokenB = st[stTop];
    int pA, pB;

    switch(tokenA) {
        case '+':
            pA = PRIORITY_ADD;
            break;
        case '-':
            pA = PRIORITY_SUB;
            break;
        case '*':
            pA = PRIORITY_MULT;
            break;
        case '/':
            pA = PRIORITY_DIV;
            break;
        case '%':
            pA = PRIORITY_MOD;
            break;
        default:
            printf("Invalid Operator.\n");
            exit(1);
    }


    switch(tokenB) {
        case '+':
            pB = PRIORITY_ADD;
            break;
        case '-':
            pB = PRIORITY_SUB;
            break;
        case '*':
            pB = PRIORITY_MULT;
            break;
        case '/':
            pB = PRIORITY_DIV;
            break;
        case '%':
            pB = PRIORITY_MOD;
            break;
        case '(':
            pB = 0;
            break;
        default:
            printf("Invalid Operator.\n");
            exit(1);
    }

    if(pA > pB)
    {
        return true;
    }else{
        return false;
    }
}


long long pop1()
{
    long long tmp;
    tmp = stack[stackTop];
    stackTop--;
    return tmp;
}

void push1(long long data)
{
    stackTop++;
    stack[stackTop] = data;
}


void cal(char operator)
{
    long long op1, op2, rst;
    op2 = pop1();
    op1 = pop1();
    switch(operator) {
        case '+':
            rst = op1 + op2;
            break;
        case '-':
            rst = op1 - op2;
            break;
        case '*':
            rst = op1 * op2;
            break;
        case '/':
            rst = op1 / op2;
            break;
        case '%':
            rst = op1 % op2;
            break;
        default:
            printf("Invalid Operator.\n");
            exit(1);
    }
    push1(rst); 
}



char pop2()
{   //pop2
    char tmp;
    tmp = st[stTop];
    stTop--;
    return tmp;
}


void push2(char c)
{
    stTop++;
    st[stTop] = c;
}

int main(int argc, char ** argv)
{
    //+ − ∗ / %
    char c;
    long long tmpData;
    for(int i = 0; i < 3; i++)
    {
        Index = 0;
        stTop = stackTop = -1;
        while((c = getchar()) != '\n')
        {
            buf1[Index] = c;
            Index++;
        }
        buf1[Index] = c;
        charCount = Index + 1;

        for(Index = 0; Index <= charCount - 1; Index++)
        {
            if(Index == charCount - 1)
            {
                while(!stEmpty())
                {
                    char tmp;
                    tmp = pop2();
                    printf("%c", tmp);
                    cal(tmp);
                }
                printf("=%lld\n", pop1());
            }

            if(buf1[Index] <= 57 && buf1[Index] >= 48)
            {
                tmpData = 0;
                while(buf1[Index] <= 57 && buf1[Index] >= 48)
                {
                    tmpData = tmpData * 10;
                    tmpData += (buf1[Index] - 48);
                    Index++;
                }
                printf("%lld", tmpData);
                push1(tmpData);
                Index--;
            }else if(buf1[Index] == '(')
            {
                push2(buf1[Index]);
            }else if(buf1[Index] == ')'){
                while(!stEmpty() && (c = pop2(st)) != '(')
                {
                    printf("%c", c);
                    cal(c);
                }
            }else{
                if(stEmpty() || stPeep() == '(')
                {
                    push2(buf1[Index]);
                }else{
                    if(priorityLargeThan(buf1[Index]))
                    {
                        push2(buf1[Index]);
                    }else{
                        do{
                            c = pop2(st);
                            printf("%c", c);
                            cal(c);
                            if(stEmpty() || priorityLargeThan(buf1[Index])){
                                break;
                            }
                        }while(1);
                        push2(buf1[Index]);
                    }
                }
            }   
        }
    }
    exit(0);
}