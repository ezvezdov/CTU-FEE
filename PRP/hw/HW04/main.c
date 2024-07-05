#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000000

//This function makes array of prime numbers array using Sieve of Eratosthenes algorithm
void make_prime_numbers_array(int *prime_numbers){
  int sieve_of_eratosthenes[ARRAY_SIZE] = {0};
  int index = 0;
  int i,j;
  for(i = 2; i < ARRAY_SIZE; i++){
    if(sieve_of_eratosthenes[i] == 1){
      continue;
    }
    prime_numbers[index] = i;
    index++;
    for(j = 0; j < ARRAY_SIZE;j+=i){
      sieve_of_eratosthenes[j] = 1;
    }
  }
  return;
}

int main(int argc, char *argv[])
{
  int prime_numbers[ARRAY_SIZE];
  make_prime_numbers_array(prime_numbers);
  
  long long int input_num = -1;
  int ret = -1;
  int index = 0;
  int div_counter = 0;

  while (input_num != 0){

    ret = scanf("%lld", &input_num);
  
    if(ret != 1 || input_num < 0){
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
    if(input_num == 0){
      break;
    }

    printf("Prvociselny rozklad cisla %lld je:\n", input_num);
    if(input_num == 1){
      printf("1");
    }

    index = 0;
    div_counter = 0;
    while (input_num != 1)
    {
      while(input_num % prime_numbers[index] == 0){
        input_num /= prime_numbers[index];
        div_counter++;
      }

      if(div_counter > 0){
        printf("%d", prime_numbers[index]);
        if(div_counter > 1){
          printf("^%d",div_counter);
        }
        if(input_num != 1){
          printf(" x ");
        }
        div_counter = 0;
      }
      index++;
    }
    printf("\n");
    
  }
  
  return 0;
}
