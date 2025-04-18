#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_VARS 100

struct Variable {
    char name[100];
    int value;              // for int
    char charValue;         // for char
    char strValue[100];     // for string
    int isChar;             // 0 = int, 1 = char, 2 = string
};

struct Variable vars[MAX_VARS];
int varCount = 0;

// Find variable index
int getVarIndex(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Evaluate simple expression (only supports int + int, int - int, etc.)
int evaluateExpression(char *expr) {
    int a, b;
    char op;
    sscanf(expr, "%d %c %d", &a, &op, &b);
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b ? a / b : 0;
        default: return a;
    }
}

// Add variable to symbol table
void declareVariables(char *line) {
    int isCharType = 0, isStringType = 0, isIntType = 0;

    printf("%s", line);

    if (strstr(line, "char") != NULL) {
        if (strchr(line, '[')) {
            isStringType = 1;
        } else {
            isCharType = 1;
        }
    } else if (strstr(line, "int") != NULL) {
        isIntType = 1;
    }

    char *declaration = strchr(line, ' ');
    if (!declaration) return;
    declaration++;

    char *token = strtok(declaration, " ,;\n");
    while (token) {
        if (strchr(token, '[')) {
            token[strcspn(token, "[")] = '\0';  // remove [size]
            isStringType = 1;
        }

        strcpy(vars[varCount].name, token);
        if (isStringType) {
            vars[varCount].isChar = 2;
            vars[varCount].strValue[0] = '\0';
        } else if (isCharType) {
            vars[varCount].isChar = 1;
            vars[varCount].charValue = '\0';
        } else if (isIntType) {
            vars[varCount].isChar = 0;
            vars[varCount].value = 0;
        }

        varCount++;
        token = strtok(NULL, " ,;\n");
    }
}

// Handle assignment
void handleAssignment(char *line) {
    char varName[100], expr[100];
    if (sscanf(line, "%[^=]= %[^\n;]", varName, expr) != 2) return;

    int idx = getVarIndex(varName);
    if (idx == -1) {
        printf("Error: undeclared variable %s\n", varName);
        return;
    }

    // Remove trailing semicolon
    expr[strcspn(expr, ";")] = '\0';

    if (vars[idx].isChar == 0) { // int
        vars[idx].value = evaluateExpression(expr);
    } else if (vars[idx].isChar == 1) { // char
        if (expr[0] == '\'' && expr[2] == '\'')
            vars[idx].charValue = expr[1];
    } else if (vars[idx].isChar == 2) { // string
        if (expr[0] == '"') {
            char temp[100];
            strcpy(temp, expr);
            char *start = strchr(temp, '"');
            char *end = strrchr(temp, '"');
            if (start && end && end > start) {
                *end = '\0';
                strcpy(vars[idx].strValue, start + 1);
            }
        }
    }
}

// Handle printf
void handlePrintf(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    if (!start || !end) return;

    char content[200];
    strncpy(content, start + 1, end - start - 1);
    content[end - start - 1] = '\0';

    char *token = strtok(content, ",");
    while (token) {
        token = strtok(token, "\" \n");
        int idx = getVarIndex(token);
        if (idx != -1) {
            if (vars[idx].isChar == 0)
                printf("%d", vars[idx].value);
            else if (vars[idx].isChar == 1)
                printf("%c", vars[idx].charValue);
            else if (vars[idx].isChar == 2)
                printf("%s", vars[idx].strValue);
        } else {
            printf("%s", token);
        }
        token = strtok(NULL, ",");
    }
    printf("\n");
}

// Handle scanf
void handleScanf(char *line) {
    char *start = strchr(line, '&');
    if (!start) return;

    char varName[100];
    sscanf(start + 1, "%s", varName);
    int idx = getVarIndex(varName);
    if (idx == -1) {
        printf("Error: variable %s not declared\n", varName);
        return;
    }

    if (vars[idx].isChar == 0) {
        scanf("%d", &vars[idx].value);
    } else if (vars[idx].isChar == 1) {
        getchar(); // Clear buffer
        scanf("%c", &vars[idx].charValue);
    } else if (vars[idx].isChar == 2) {
        scanf("%s", vars[idx].strValue);
    }
}

// Main interpreter
void custom_fun(FILE *input) {
    char line[200];
    bool start = false;

    while (fgets(line, sizeof(line), input)) {
        if (strstr(line, "int main")) break;
        if (!start && strstr(line, "void") && strchr(line, '(')) {
            start = true;
            continue;
        }
        if (!start) continue;

        if (strstr(line, "int ") || strstr(line, "char ")) {
            declareVariables(line);
        } else if (strstr(line, "scanf")) {
            handleScanf(line);
        } else if (strstr(line, "printf")) {
            handlePrintf(line);
        } else if (strchr(line, '=')) {
            handleAssignment(line);
        }
    }
}

int main() {
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        printf("Error opening file.\n");
        return 1;
    }

    custom_fun(input);

    fclose(input);
    return 0;
}
