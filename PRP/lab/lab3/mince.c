#include<stdio.h>

int main(){
    int amount = 0;
    scanf("%d", &amount);
    /*
    printf("20 - %d\n",amount / 20);
    amount = amount % 20;
    printf("10 - %d\n",amount / 10);
    amount = amount % 10;
    printf("5 - %d\n",amount / 5);
    amount = amount % 5;
    printf("2 - %d\n",amount / 2);
    amount = amount % 2;
    printf("1 - %d\n",amount);
    */

   int mince[5] = {20,10,5,2,1};
   int cnt[5] = {0,0,0,0,0};
   int i = 0;
   while(amount != 0){
        if(amount / mince[i] >= 1 ){
            cnt[i]+= amount / mince[i];
            amount %= mince[i];
        }
        else{
            i++;
        }
   }
   for(int i =0;i < 5; i++){
       printf("mince %d - %d\n", mince[i],cnt[i]);
   }
    return 0;

}