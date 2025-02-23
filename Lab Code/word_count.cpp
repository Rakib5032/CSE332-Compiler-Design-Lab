#include<bits/stdc++.h>

int main(){
	char ch[100];
	fgets(ch, sizeof(ch), stdin);
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
	printf("%d", ans);
}
