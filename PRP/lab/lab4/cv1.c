#include<stdio.h>

int main(){
    int n = 0;
    int x = -1;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        x = -x;
        printf("%d ", x);
    }
}