#include<bits/stdc++.h>

int main(){

	FILE *input = fopen("input_wc.txt", "r");
	FILE *output = fopen("output_wc.txt", "w");


	char ch[100];
	fgets(ch, sizeof(ch), input);
	int cnt = 0, ans  = 0;
	bool fl = false;
	for(int i = 0; i < 100; i++){
		if(ch[i] == '\0' || ch[i] == '\n'){
			if(cnt == 0 && fl){
				ans++;
			} break;
		} else if(((ch[i] >= 'a' && ch[i] <= 'z') || (ch[i] >= 'A' && ch[i] <= 'Z'))){
			fl = true;
		} else if(ch[i] == '\'' || ch[i] == '-'){
			continue;
		} else if(ch[i] == ' ' || ch[i] == '.' || ch[i] == '!' || ch[i] == '?'){
			if(cnt == 0 && fl){
				ans++;
			}
			cnt = 0;
			fl = false;
		} else{
			cnt++;
		}
	}
	fprintf(output, "%d", ans);
	// fprintf("%d", ans);
}
