#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int num_count = 0, sum = 0;
  int positive = 0, negative = 0, even = 0, odd = 0, max_num, min_num;
  int ret = 1;

  while (ret == 1){
    int input_num;
    ret = scanf("%d",&input_num);

    if(ret != 1){
      break;
    }

    if(input_num < -10000 || input_num > 10000){
      printf("\nError: Vstup je mimo interval!\n");
      return 100;
    }

    if(num_count == 0){
      printf("%d", input_num);
    }
    else{
      printf(", %d",input_num);
    }

    if(num_count == 0){
      max_num = input_num;
      min_num = input_num;
    }
    
    if(input_num > 0){
      positive++;
    }
    else if(input_num < 0){
      negative++;
    }

    if(input_num % 2 == 0){
      even++;
    }
    else{
      odd++;
    }

    if(input_num > max_num){
      max_num = input_num;
    }
    if(input_num < min_num){
      min_num = input_num;
    }

    num_count++;
    sum+=input_num;
  }

  printf("\nPocet cisel: %d\n", num_count);
  printf("Pocet kladnych: %d\n", positive);
  printf("Pocet zapornych: %d\n", negative);
  printf("Procento kladnych: %.2f\n", ((double) positive * 100) / num_count);
  printf("Procento zapornych: %.2f\n", ((double) negative * 100) / num_count);
  printf("Pocet sudych: %d\n",even);
  printf("Pocet lichych: %d\n", odd);
  printf("Procento sudych: %.2f\n", ((double) even * 100) / num_count);
  printf("Procento lichych: %.2f\n", ((double) odd * 100) / num_count);
  printf("Prumer: %.2f\n", (double) sum / num_count);
  printf("Maximum: %d\n", max_num);
  printf("Minimum: %d\n", min_num);

  return 0;
}
