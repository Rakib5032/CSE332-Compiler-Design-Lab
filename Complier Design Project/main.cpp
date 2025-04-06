#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VAR 100

typedef struct {
    char name[50];
    int value;
} Variable;

Variable vars[MAX_VAR];
int varCount = 0;

// Remove trailing characters like ; or )
void clean(char *str) {
    str[strcspn(str, ");\n")] = 0;
}

// Add variable to symbol table
void declareVariables(char *line) {
    char *token = strtok(line + 3, " ,;\n");
    while (token) {
        strcpy(vars[varCount].name, token);
        vars[varCount].value = 0;
        varCount++;
        token = strtok(NULL, " ,;\n");
    }
}

// Find variable by name
int getVarIndex(const char *name) {
    for (int i = 0; i < varCount; i++)
        if (strcmp(vars[i].name, name) == 0)
            return i;
    return -1;
}

// Handle a = b + 5; or a = 5;
void handleAssignment(char *line) {
    char var[50], op1[50], op[5], op2[50];
    int idx, val;

    if (sscanf(line, "%s = %s %s %s", var, op1, op, op2) == 4) {
        idx = getVarIndex(var);
        int a = isdigit(op1[0]) ? atoi(op1) : vars[getVarIndex(op1)].value;
        int b = isdigit(op2[0]) ? atoi(op2) : vars[getVarIndex(op2)].value;

        if (strcmp(op, "+") == 0) val = a + b;
        else if (strcmp(op, "-") == 0) val = a - b;
        else if (strcmp(op, "*") == 0) val = a * b;
        else if (strcmp(op, "/") == 0) val = a / b;

        vars[idx].value = val;
    } else if (sscanf(line, "%s = %d", var, &val) == 2) {
        vars[getVarIndex(var)].value = val;
    } else if (sscanf(line, "%s = %s", var, op1) == 2) {
        vars[getVarIndex(var)].value = vars[getVarIndex(op1)].value;
    }
}

// Handle scanf("%d", &a);
void handleScanf(char *line) {
    char var[50];
    sscanf(line, "scanf(\"%%d\", &%[^)]", var);
    clean(var);
    int idx = getVarIndex(var);
    printf("Input value for %s: ", var);
    scanf("%d", &vars[idx].value);
}

// Handle printf("%d", b);
void handlePrintf(char *line) {
    char var[50];
    sscanf(line, "printf(\"%%d\", %[^)]", var);
    clean(var);
    printf("%d\n", vars[getVarIndex(var)].value);
}

// Entry point
int main() {
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        printf("Failed to open input.txt\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        if (strstr(line, "int main")) continue;
        if (strstr(line, "return")) break;
        if (strstr(line, "int ")) declareVariables(line);
        else if (strstr(line, "scanf")) handleScanf(line);
        else if (strstr(line, "printf")) handlePrintf(line);
        else if (strchr(line, '=')) handleAssignment(line);
    }

    fclose(input);
    return 0;
}
