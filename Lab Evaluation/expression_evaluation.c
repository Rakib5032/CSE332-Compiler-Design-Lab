#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

double fun(double a, double b, char c){
    if(c == '/') return a / b;
    if(c == '*') return a * b;
    if(c == '+') return a + b;
    if(c == '-') return a - b;
}
int main(){
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    char ch1[100];
    char ch2[100];

    fgets(ch1, sizeof(ch1), input);
    fgets(ch2, sizeof(ch2), input);

    printf("%s", ch1);

    char temp[100];
    int ara[5];
    int k = 0;
    int t = 0;
    for(int i = 0; ch1[i] != '\0'; i++){
        if(ch1[i] >= '0' && ch1[i] <= '9'){
            temp[k++] = ch1[i];
        }
        else{
            temp[k] = '\0';
            ara[t++] = atoi(temp);
            k = 0;
        }
    }

    double ans = 0;
    char ch[2];
    
    k = 0;
    for(int i = 0; ch2[i] != '\0'; i++){
        if(ch2[i] == '-' || ch2[i] == '+' || ch2[i] == '*' || ch2[i] == '/'){
            ch[k++] = ch2[i];
        }
    }

    if((ch[0] == '+' || ch[0] == '-') && (ch[1] == '/' || ch[1] == '*')){
        ans = fun((double)ara[1], (double)ara[2], ch[1]);
        ans = fun((double)ara[0], ans, ch[0]);
        
    }
    
    else{
        ans = fun((double)ara[0], (double)ara[1], ch[0]);
        ans = fun(ans, (double)ara[2], ch[1]);
    }
    printf("\n%lf", ans);

    fprintf(output, "%lf",ans);
}