#include <stdio.h>
#include <ctype.h>

void extractVariableName(char *line, char *varName) {
    int i = 0;
    // Skip initial spaces
    
}

int main() {
    char line[] = "b = 1000;";
    char var[50];

    extractVariableName(line, var);
    printf("Variable name: %s\n", var);

    return 0;
}
