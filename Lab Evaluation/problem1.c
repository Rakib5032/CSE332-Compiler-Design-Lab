/*
String Operation (Slicing, Concatenation, compare, sub-String findout)
*/

#include<stdio.h>
#include<string.h>

int main(){

    FILE *input = fopen("input_problem1.txt", "r");
    FILE *output = fopen("output_problem1.txt", "w");

    char ch1[100];
    char ch2[100];

    // for single word
    // fscanf(input, stdin, &ch1);
    // fscanf(input, stdin, &ch2);

    fgets(ch1, sizeof(ch1), input);
    fgets(ch2, sizeof(ch2), input);


    // Concatenation

    char result[2000];
    
    // built-in function
    // strcpy(result, ch1), 
    // strcat(result, ch2);

    int l = strlen(ch1);
    int j = 0;
    for(int i = 0; i < l; i++){
        result[j++] = ch1[i];
    }
    printf("%d\n", l);
    l += strlen(ch2);
    printf("%d\n", l);

    int k = 0;
    for(int i = j; i < l; i++){
        result[j++] = ch2[k++];
    }

    fprintf(output, "%s\n\n", result);


    // fprintf(output, "%s", ch1);
    // fprintf(output, "%s", ch2);


    return 0;
}