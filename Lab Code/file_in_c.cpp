#include <stdio.h>

int main() {
    FILE *input = fopen("input.txt", "r");  // Open input file for reading
    FILE *output = fopen("output.txt", "w"); // Open output file for writing

    if (input == NULL || output == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int n;
    // Read from input file
    fscanf(input, "%d", &n);

    // Write to output file
    fprintf(output, "Read number: %d\n", n);

    // Close the files
    fclose(input);
    fclose(output);

    return 0;
}
