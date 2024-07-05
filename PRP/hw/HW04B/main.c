#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define END_OF_STRING '\0'
#define ZERO_CHAR '0'
#define PRIME_LIMIT 1000000

typedef enum {false, true} bool;

void check_input(char *num){
    for(int i = 0; i < strlen(num); i++){
        if(num[i] < ZERO_CHAR || num[i] > '9'){
            fprintf(stderr, "Error: Chybny vstup!\n"); 
            exit(100);
        }
    }
}

/*Input number into char array and reverse it.*/
void input_number(char *not_reversed_num, char *num){
    if(scanf("%s",not_reversed_num) != 1){
        exit(100);
    }
    int i = strlen(not_reversed_num) - 1;

    for(int j = 0; i >= 0; j++){
        num[j] = not_reversed_num[i];
        i--;
    }
}

/*Returns true if number equals to 1.*/
bool is_one(char *num){
    if(strlen(num) == 0 || num[0] != '1'){
        return false;
    }
    for(int i = 1; num[i] != END_OF_STRING; i++){
        if(num[i] != ZERO_CHAR){
            return false;
        }
    }
    return true;
}

/*Returns true if number equals to 0.*/
bool is_zero(char *num){
    if(strlen(num) == 0 || num[0] != ZERO_CHAR){
        return false;
    }
    for(int i = 1; num[i] != END_OF_STRING; i++){
        if(num[i] != ZERO_CHAR){
            return false;
        }
    }
    return true;
}

/*Division operation.*/
void division(char* num, int b){
    int carry = 0;
    for(int i = strlen(num) - 1 ; i >= 0; i--){
        int cur = num[i] - ZERO_CHAR + carry * 10;
        num[i] = ZERO_CHAR + (cur / b);
        carry = cur % b;
    }
}

/*Module operation.*/
int mod(char* num, int b){
    char num_copy[120];
    strcpy(num_copy,num);
    int carry = 0;
    for(int i = strlen(num_copy) - 1 ; i >= 0; i--){
        int cur = num_copy[i] - ZERO_CHAR + carry * 10;
        num_copy[i] = ZERO_CHAR + (cur / b);
        carry = cur % b;
    }
    return carry;
}

/*Returns true if number is prime.*/
bool is_prime(int a){
    for(int i = 2; i < sqrt(a); i++){
        if(a % i == 0){
            return false;
        }
    }
    return true;
}

/*Find num primes.*/
void find_primes(char* num){
    int division_counter = 0;

    for(int i = 2; i < PRIME_LIMIT && !is_one(num); i++){
        if(!is_prime(i)){
            continue;
        }
        
        while (mod(num,i) == 0)
        {
            division(num,i);
            division_counter++;
        }
        
        if(division_counter > 0){
            printf("%d", i);
            if(division_counter > 1){
                printf("^%d",division_counter);
            }
            if(!is_one(num)){
                printf(" x ");
            }
        }
        
        division_counter = 0;
    }
}




int main(int argc, char *argv[])
{
    char not_reversed_num[120] = "";
    char num[120] = "";
  
    while (true){
        input_number(not_reversed_num,num);
        check_input(num);

        if(is_zero(num)){
            break;
        }

        printf("Prvociselny rozklad cisla %s je:\n", not_reversed_num);
        if(is_one(num)){
            printf("1");
        }

        find_primes(num);
        printf("\n");
    }
  
    return 0;
}
