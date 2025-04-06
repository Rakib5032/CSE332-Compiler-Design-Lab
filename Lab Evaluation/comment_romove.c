
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

int main(){
    FILE *input = fopen("input_comment.txt", "r");
    FILE *output = fopen("output_comment.txt", "w");

    bool fl = false;
    int k = 0;
    char ch[150];

    // taking input of each line from the input file
    while(fgets(ch, sizeof(ch), input)){

        char temp[150];
        k = 0;

        for(int i = 0; ch[i] != '\0'; i++){
            // if any comment is found then skip the line, comment is like this line
            // Single line comment
            // single line comment
            if(ch[i] == '/' && ch[i+1] == '/'){ //asfasf
                break;
            }

            /* comment like this will be handeled here
            hello comment
            hello world
            multiline comment
            this four line ocmment will be removed if it is in input
            */
            else if(ch[i] == '/' && ch[i+1] == '*'){
                fl = true;
                i++;
                continue;
            }

            /*checking if the comment is end or not*/
            else if(fl == true && ch[i] == '*' && ch[i+1] == '/'){
                fl = false;
                i++;
                continue;
            }
            if(fl == true){
                continue;
            }
            temp[k++] = ch[i];

        }
        temp[k] = '\0';

        // int t = 0;
        // int i = 0;

        /*while(temp[t] != '\0'){
            if(temp[t++] == ' '){
                i++;
            }
        }

        printf("%d %d\n", i, t);
        if(temp[0] != '\0' && i != t){
            fprintf(output, "%s", temp);
        }*/

        if(temp[0] != '\0'){
            fprintf(output, "%s", temp);
        }

    }
    fclose(input);
    fclose(output);
    return 0;
}