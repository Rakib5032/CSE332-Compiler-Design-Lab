#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 100

int precedence(char oper)
{
    switch(oper)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return -1;
    }
}

int isOperator(char ch)
{
    return(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='^');
}

char* infixtopostfix(char* infix)
{
    int i, j, top=-1;
    int len = strlen(infix);
    char* postfix = (char*) malloc(sizeof(char) * (len+2));
    char stack[MAX_SIZE];

    for(i=0, j=0; i<=len; i++)
    {
        if(infix[i]==' ' || infix[i]=='\t')
        {
            continue;
        }
        else if(isalnum(infix[i]))
        {
            postfix[j++]=infix[i];
        }
        else if(infix[i] == '(')
        {
            stack[++top] = infix[i];
        }
        else if(infix[i] == ')')
        {
            while(top>-1 && stack[top]!='(')
            {
                postfix[j++]=stack[top--];
            }
            top--;
        }
        else if(isOperator(infix[i]))
        {
            while(top>-1 && precedence(stack[top])>=precedence(infix[i]))
            {
                postfix[j++] = stack[top--];
            }
            stack[++top]=infix[i];
        }
    }

    while(top>-1)
    {
        postfix[j++] = stack[top--];
    }
    postfix[j]='\0';
    return postfix;
}

int main()
{
    char infix[MAX_SIZE];
    fgets(infix, sizeof(infix), stdin);
    char* postfix = infixtopostfix(infix);
    printf("%s", postfix);
    return 0;
}
