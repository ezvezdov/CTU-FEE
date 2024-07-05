#include<stdio.h>

int main(){
    double num = 0;
    int delitel = 1;
    int cislitel = 4;
    for(int i = 0; i < 100000000; i++){
        num += (double)cislitel / delitel;
        delitel += 2;
        cislitel = -cislitel;
    }
    printf("%f", num);
}