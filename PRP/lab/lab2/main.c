#include<stdio.h>

int main(){
    int in, sc;
    while(1){
        sc = scanf("%d",&in);
        printf("Print sc: %d\n", sc);
        if(sc == EOF){
            break;
        }
        if(sc != 1){
            fprintf(stderr, "Cannot read frome srdin!");
            return 1;
        }
        printf("Number: %d\n",in);
    }
    

}