#include <stdio.h>

int main(){
    int i, j;
    int n;

    printf("整数を入力してください。\n");
    scanf("%d", &n);

    //1行目→n行目まで
    for(i=1;i<=n;i++){
        for(j=1;j<=i;j++){
            if(j%2 != 0){ 
                printf("*"); // i行目の奇数目の文字
            }else{
                printf("#"); // i行目の偶数目の文字
            }
        }
        printf("\n");
    }

    //n+1行目→最終行まで
    for(i=(n-1);i>=0;i--){
        for(j=1;j<=i;j++){
            if(j%2 != 0){ 
                printf("*"); // 2n-i行目の奇数目の文字
            }else{
                printf("#"); // 2n-i行目の偶数目の文字
            }
        }
        printf("\n");
    }

    return 0;
}