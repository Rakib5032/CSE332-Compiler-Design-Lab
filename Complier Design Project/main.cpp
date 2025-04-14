#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VAR 100

typedef struct {
    char name[50];
    char value;
} Variable;

Variable vars[MAX_VAR];
int varCount = 0;

// Remove trailing characters like ; or )
void clean(char *str) {
    str[strcspn(str, ");\n")] = 0;
}

void removespace(char *str) {
    while (isspace(*str)) str++;  // Skip all leading spaces
}

// Add variable to symbol table
void declareVariables(char *line) {

    removespace(line);

    char *token = strtok(line + 3, " ,;\n");
    printf("%s\n", token);
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
    char format_str[100];
    char var_names[10][50];
    int var_count = 0;

    bool in_format = false, after_format = false;
    int i = 0, format_idx = 0;

    // Extract format string and variable names
    while (line[i] != '\0' && line[i] != ';') {
        if (line[i] == '"' && !in_format) {
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

    // Parse format specifiers
    char *fmt_ptr = format_str;
    int v = 0;

    while (*fmt_ptr && v < var_count) {
        if (*fmt_ptr == '%' && *(fmt_ptr + 1)) {
            int idx = getVarIndex(var_names[v]);
            if (idx == -1) {
                printf("Variable %s not declared.\n", var_names[v]);
            } else if (*(fmt_ptr + 1) == 'd') {
                printf("Enter integer for %s: ", var_names[v]);
                scanf("%d", &vars[idx].value);
            } else if (*(fmt_ptr + 1) == 's') {
                // optional string input
                char temp[100];
                printf("Enter string for %s: ", var_names[v]);
                scanf("%s", temp);
                // store string if needed
            }
            v++;
            fmt_ptr++; // skip next char too
        }
        fmt_ptr++;
    }
}

// Handle printf
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
        if (format_str[j] == '%' && (format_str[j + 1] == 'd')) {
            int idx = getVarIndex(var_names[v]);
            if (idx != -1) {
                printf("%d", vars[idx].value);
            } else {
                printf("Error");
            }
            v++;
            j++;
        } else {
            putchar(format_str[j]);
        }
    }

    putchar('\n');
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
