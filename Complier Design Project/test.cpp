#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 100
#define MAX_VAR 100

typedef struct {
    char name[50];
    int value;
    char charValue;
    char strValue[100]; 
    int isChar;         
} Variable;

Variable vars[MAX_VAR];
int varCount = 0;

// Remove trailing characters like { ; ) \n }
void clean(char *str) {
    str[strcspn(str, ");\n")] = 0;
}

// Add variable to symbol table
void declareVariables(char *line) {
    int isCharType = 0, isStringType = 0, isIntType = 0;

    // Detect type
    if (strstr(line, "char") != NULL) {
        if (strchr(line, '[')) {
            isStringType = 1;
        } else {
            isCharType = 1;
        }
    } else if (strstr(line, "int") != NULL) {
        isIntType = 1;
    } else {
        printf("YES got\n");
    }

    // Skip the type keyword ("int", "char", etc.)
    char *declaration = strchr(line, ' ');
    if (!declaration) return; // Invalid line
    declaration++; // Move past the space

    // Now tokenize variable names
    char *token = strtok(declaration, " ,;\n");
    while (token) {
    
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

// Find variable by name
int getVarIndex(const char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Handle scanf("%d", &a);
void handleScanf(char *line) {
    char format_str[100];
    char var_names[10][50];
    int var_count = 0;

    bool in_format = false, after_format = false;
    int i = 0, format_idx = 0;

    // Extract format string and variable names
    while (line[i] != '\0' && line[i] != ';') {
        if (line[i] == '"' && in_format == false) {
            in_format = true;
            i++;
            continue;
        } else if (line[i] == '"' && in_format) {
            in_format = false;
            after_format = true;
            i++;
            continue;
        }

        if (in_format) {
            format_str[format_idx++] = line[i];
        } else if (after_format && isalpha(line[i])) {
            int k = 0;
            while (isalnum(line[i])) {
                var_names[var_count][k++] = line[i++];
            }
            var_names[var_count][k] = '\0';
            var_count++;
            continue;
        }

        i++;
    }
    format_str[format_idx] = '\0';

    int v = 0;
    for (int j = 0; format_str[j] != '\0'; j++) {
        if (format_str[j] == '%') {
            int idx = getVarIndex(var_names[v]);
            if (idx != -1) {
                if (format_str[j + 1] == 'd') {
                    scanf("%d", &vars[idx].value);
                }
                else if (format_str[j + 1] == 'c') {
                    //fgets(&vars[idx].charValue, sizeof(&vars[idx].charValue), stdin);
                    getchar();
                    scanf("%c", &vars[idx].charValue);
                    
                }
                else if (format_str[j + 1] == 's') {
                    getchar();
                    scanf("%s", vars[idx].strValue);
                }
            }
            else {
                printf("Variable %s not declared.\n", var_names[v]);
            }
            v++;
            j++;
        }
        else {
            putchar(format_str[j]);
        }
    }
}

void handlePrintf(char *line) {
    char var_names[10][50];
    char format_str[100];
    int var_count = 0;

    bool in_format = false, after_format = false;
    int i = 0, formate_idx = 0;

    // Extract format string and variable names
    while (line[i] != '\0' && line[i] != ';') {
        if (line[i] == '"' && in_format == false) {
            in_format = true;
            i++;
            continue;
        }
        else if (line[i] == '"' && in_format) {
            in_format = false;
            after_format = true;
            i++;
            continue;
        }

        if (in_format) {
            format_str[formate_idx++] = line[i];
        } 
        else if (after_format && isalpha(line[i])) {
            int k = 0;
            while (isalnum(line[i])) {
                var_names[var_count][k++] = line[i++];
            }
            var_names[var_count][k] = '\0';
            var_count++;
            continue;
        }

        i++;
    }
    format_str[formate_idx] = '\0';

    // Print formatted output
    int v = 0;
    for (int j = 0; format_str[j] != '\0'; j++) {
        if (format_str[j] == '%') {
            int idx = getVarIndex(var_names[v]);
            if (idx != -1) {
                if (format_str[j + 1] == 'd') {
                    printf("%d", vars[idx].value);
                }
                else if (format_str[j + 1] == 'c') {
                    printf(" %c", vars[idx].charValue);
                } 
                else if (format_str[j + 1] == 's') {
                    printf("%s", vars[idx].strValue);
                }
            }
            else {
                printf("Variable %s not declared.\n", var_names[v]);
            }
            v++;
            j++;
        }
        else {
            putchar(format_str[j]);
        }
    }

    putchar('\n');
}

void handleAssignment(char *line) {
    char varName[50], op1[50], op2[50], opChar;
    int varIndex;

    printf("%s\n", line);
    // Remove trailing characters
    clean(line);
    printf("%s\n", line);

    // Case 1: Arithmetic Expression (e.g., a = b + c;)
    if (sscanf(line, "%s = %s %c %s", varName, op1, &opChar, op2) == 4) {
        varIndex = getVarIndex(varName);
        if (varIndex == -1) {
            printf("Variable %s not declared.\n", varName);
            return;
        }

        int val1 = isalpha(op1[0]) ? vars[getVarIndex(op1)].value : atoi(op1);
        int val2 = isalpha(op2[0]) ? vars[getVarIndex(op2)].value : atoi(op2);

        switch (opChar) {
            case '+': vars[varIndex].value = val1 + val2; break;
            case '-': vars[varIndex].value = val1 - val2; break;
            case '*': vars[varIndex].value = val1 * val2; break;
            case '/': vars[varIndex].value = (val2 != 0) ? val1 / val2 : 0; break;
            default: printf("Unsupported operator: %c\n", opChar);
        }
    }
    // Case 2: Simple assignment (e.g., x = 5;)
    else if (sscanf(line, "%s = %s", varName, op1) == 2) {
        varIndex = getVarIndex(varName);
        if (varIndex == -1) {
            printf("Variable %s not declared.\n", varName);
            return;
        }

        int srcIndex = getVarIndex(op1);
        if (vars[varIndex].isChar == 0) {  // int
            vars[varIndex].value = (srcIndex != -1) ? vars[srcIndex].value : atoi(op1);
        }
        else if (vars[varIndex].isChar == 1) {  // char
            vars[varIndex].charValue = (srcIndex != -1) ? vars[srcIndex].charValue : op1[0];
        }
        else if (vars[varIndex].isChar == 2) {  // string
            if (op1[0] == '"') {
                // remove surrounding quotes
                memmove(op1, op1 + 1, strlen(op1));
                op1[strlen(op1) - 1] = '\0';
            }
            strcpy(vars[varIndex].strValue, (srcIndex != -1) ? vars[srcIndex].strValue : op1);
        }
    }
    else {
        printf("Invalid assignment: %s\n", line);
    }
}

void custom_fun(FILE *input) {
    char line[256];
    bool start = false;

    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = 0;  // remove newline

        if (!start && strstr(line, "void")) {
            start = true;
            continue;
        }

        if (start) {
            if (strstr(line, "int ")) {
                declareVariables(line);
            } else if (strstr(line, "char ")) {
                declareVariables(line);
            } else if (strstr(line, "scanf")) {
                handleScanf(line);
            } else if (strstr(line, "printf")) {
                handlePrintf(line);
            } else if (strchr(line, '=')) {
                handleAssignment(line);
            }
            if (strstr(line, "int mian")) {
                break;
            }
        }
    }
}

int main() {
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        printf("Failed to open input.txt\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        if (strstr(line, "//")) continue;
        else if (strstr(line, "int main")) continue;
        else if (strstr(line, "return")) break;
        if (strstr(line, "int ")) declareVariables(line);
        if(strstr(line, "char ")) declareVariables(line);
        if (strstr(line, "scanf")) handleScanf(line);
        if (strstr(line, "printf")) handlePrintf(line);
        if (strchr(line, '=')) handleAssignment(line);
        //else if (strstr(line, "()" && strstr(line, "void") == N) custom_fun(input);
    }

    fclose(input);
    return 0;
}