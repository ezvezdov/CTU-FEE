#include<stdio.h>

int main(){
    int a;
    int ret = scanf("%d",&a);
    if(ret <= 0){
        printf("\n%d", ret);
        return 1;
    }
    return 0;
}