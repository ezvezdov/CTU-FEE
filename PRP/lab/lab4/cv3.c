#include<stdio.h>

int main(){
    int n = 0;
    for(int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            printf("%3d", n);
            n++;
        }
        printf("\n");
    }
}