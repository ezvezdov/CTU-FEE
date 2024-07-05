#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 100 // default string size
#define SHIFT_COUNT 52 //count of letters from A to Z + a to z

void allocation_memory_error(){
    fprintf(stderr,"Error: Memory allocation error!\n");
    exit(1);
}

/*find min*/
int min(int num1, int num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}

/*reallocating memmrory for input string*/
char* reallocating_memmory(char *str, int capacity){
    char *new_string = NULL;
    new_string = realloc(str, capacity);
    if(new_string == NULL){
        allocation_memory_error();
    }
    str = new_string;
    return str;
}

/*get string (all characters on line) from input*/
char* getline(int *len){
    char in_char;
    int capacity = SIZE;
    char *str = NULL;
    str = malloc(capacity);
    if(str == NULL){
        allocation_memory_error();
    }

    while( (in_char = getchar()) != EOF && in_char != '\n'){
        if(in_char < 'A' || (in_char > 'Z' && in_char < 'a') || in_char > 'z'){
            fprintf(stderr, "Error: Chybny vstup!\n");
            exit(100);
        }
        str[*len] = in_char;
        (*len)++;
        if(*len == capacity){
            capacity *= 2;
            str = reallocating_memmory(str,capacity);
        }
        str[*len] = '\0';
    }

    return str;
}

/*makes char shift*/
void shift(char *str, int len, int offset){
    for(int j = 0; j < offset; j++){
        for(int i = 0; i < len; i++){
            if(str[i] == 'Z'){
                str[i] = 'a';
            }
            else if(str[i] == 'z'){
                str[i] = 'A';
            }
            else{
                str[i]++;
            }
        }
    }
    
}

/*compare 2 strings*/
int compare(char *str1, char *str2,int length,int tmp){
    int matched_count = 0;
    for(int i = 0; i < length; i++){
        if(str1[i] == str2[i]){
            matched_count++;
        }
    }
    return matched_count;
}

/*Levenshtein algorithm for option with different string lengths*/
int Levenshtein(char* str1, char* str2, int len1, int len2){
    int* arr = NULL;
    arr = malloc(((len1+2) * (len2+2)) * sizeof(int));
    if(arr == NULL){
        allocation_memory_error();
    }

    for(int j = 0; j <= len2; j++){
        arr[j] = j;   
    }
        
    for(int i = 1; i <= len1+1; i++){
        arr[i * (len2 + 1)] = i;
    }
            
    for(int i = 1; i <= len1; i++){
        for(int j = 1 ; j <= len2; j++ ){
            arr[i * (len2+1) + j] = min(min(
                                    arr[i * (len2+1) + j-1],
                                    arr[(i-1) * (len2+1) + j]),
                                    arr[(i-1) * (len2+1) + j-1] 
                                ) + !(str1[i-1] == str2[j-1]);
        }
    }
    int return_value = arr[len1 * (len2+1) + len2];
    free(arr);
    return return_value;
}


int main(int argc, char *argv[])
{
    char *str1, *str2;
    int len1 = 0,len2 = 0;
    
    str1 = getline(&len1);
    str2 = getline(&len2);

    char optional_argument[] = "-prp-optional";
    if( (len1 != len2 && argc == 1) || (len1 != len2 &&  strcmp(argv[1], optional_argument) )){
        free(str1);
        free(str2);
        fprintf(stderr,"Error: Chybna delka vstupu!\n");
        return 101;
    }

    int best_compare = 0, current_compare = 0, best_match_shift = 0;
    //border value for Levenshtein algorithm
    if(len1 != len2){
        best_compare = __INT_MAX__;
    }

    for(int i = 0; i < SHIFT_COUNT; i++){
        shift(str1,len1,1);
        if(len1 == len2){
            current_compare = compare(str1,str2,len1,i);
        }
        else{
            current_compare = Levenshtein(str1,str2,len1,len2);
        }
        
        if( (len1 == len2 && current_compare > best_compare) || (len1 != len2 && current_compare < best_compare)){
            best_compare = current_compare;
            best_match_shift = i+1;
        }
    }
    shift(str1, len1,best_match_shift);
    printf("%s\n", str1);

    free(str1);
    free(str2);

    return 0;
}
