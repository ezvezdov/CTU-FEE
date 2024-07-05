#include<stdio.h>

int main(){
    int tmp;

    FILE *file = fopen("text.txt","r");

    fscanf(file, "%d", &tmp);
    printf("%d", tmp);

    fclose(file);
}