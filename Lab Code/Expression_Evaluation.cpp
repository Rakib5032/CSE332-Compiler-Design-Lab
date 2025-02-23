#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Stack structure
typedef struct {
    double arr[MAX];
    int top;
} Stack;

// Push function for the stack
void push(Stack *s, double num) {
    s->arr[++(s->top)] = num;
}

// Pop function for the stack
double pop(Stack *s) {
    if (s->top == -1) return 0;
    return s->arr[(s->top)--];
}

// Check if a character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

// Check operator precedence
int precedence(char ch) {
    if (ch == '+' || ch == '-') return 1;
    if (ch == '*' || ch == '/') return 2;
    return 0;
}

// Convert infix expression to postfix
char* infixToPostfix(char* infix) {
    int j = 0, top = -1;
    int len = strlen(infix);
    char* postfix = (char*)malloc(sizeof(char) * (len * 2));
    char stack[MAX];

    for (int i = 0; infix[i] != '\0'; i++) {
        if (infix[i] == ' ' || infix[i] == '\t') continue;
        
        // Handle multi-digit numbers
        if (isdigit(infix[i])) {
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; // Add space as separator
            i--; // Adjust index since we moved one extra step
        }
        else if (infix[i] == '(') {
            stack[++top] = infix[i];
        }
        else if (infix[i] == ')') {
            while (top > -1 && stack[top] != '(') {
                postfix[j++] = stack[top--];
            }
            top--; // Remove '('
        }
        else if (isOperator(infix[i])) {
            while (top > -1 && precedence(stack[top]) >= precedence(infix[i])) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = infix[i];
        }
    }
    
    while (top > -1) {
        postfix[j++] = stack[top--];
    }
    postfix[j] = '\0';
    return postfix;
}

// Evaluate the postfix expression
double evaluatePostfix(char *infix) {
    char* postfix = infixToPostfix(infix);
    printf("Postfix: %s\n", postfix);

    Stack s;
    s.top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        if (postfix[i] == ' ') continue;

        if (isdigit(postfix[i])) {
            double num = 0;
            // Handle multi-digit numbers
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            push(&s, num);
            i--;  // Adjust index
        }
        else if (isOperator(postfix[i])) {
            double var2 = pop(&s);
            double var1 = pop(&s);
            switch (postfix[i]) {
                case '+': push(&s, var1 + var2); break;
                case '-': push(&s, var1 - var2); break;
                case '*': push(&s, var1 * var2); break;
                case '/': push(&s, var1 / var2); break;
            }
        }
    }

    return pop(&s);
}

int main() {
    char infix[MAX];
    
    printf("Enter infix expression: ");
    fgets(infix, MAX, stdin);
    
    // Evaluate and print the result
    double result = evaluatePostfix(infix);
    printf("Result: %.2lf\n", result);

    return 0;
}
