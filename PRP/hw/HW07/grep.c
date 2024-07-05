#include <stdio.h>
#include <stdlib.h>

#define SIZE 100
#define ANSI_COLOR_RESET   "\x1b[m\x1b[K"
#define ANSI_COLOR_RED     "\x1b[01;31m\x1b[K"
#define COLOR_OPTION         "--color=always"

void allocation_memory_error(){
    fprintf(stderr,"Error: Memory allocation error!\n");
    exit(1);
}

char* reallocating_memmory(char *str, int capacity){
    char *new_string = NULL;
    new_string = realloc(str, capacity);
    if(new_string == NULL){
        allocation_memory_error();
    }
    
    return new_string;
}

/*get string (all characters on line) from input*/
char* getline(int *len, FILE *input_stream){
    char in_char;
    int capacity = SIZE;
    char *str = malloc(capacity);
    if(str == NULL){
        allocation_memory_error();
    }

    while(fscanf(input_stream, "%c",&in_char) == 1  && in_char != '\n'){
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

int check_word(char *line, int index_start, char* pattern){
    for(int i = 0; pattern[i] != '\0' && line[i] != '\0'; i++){
        if(line[index_start + i] != pattern[i]){
            return 0;
        }
    }
    return 1;
}

void print_colored_line(char *line, int start_index, int start_color,int end_color){
    for(int i = start_index; i < start_color; i++){
        printf("%c",line[i]);        
    }
    if(start_color != end_color){
        printf(ANSI_COLOR_RED);
    }
    
    for(int i = start_color; i < end_color; i++){
        printf("%c",line[i]);
    }
    if(start_color != end_color){
        printf(ANSI_COLOR_RESET);
    }
    

}

int string_compare(char *str1, char* str2){
    int i = 0;
    do{
        if(str1[i] != str2[i]){
            return 0;
        }
        i++;
    }while(str1[i] != '\0');
    return 1;

}

int main(int argc, char *argv[])
{
    char *options = NULL;
    char *pattern = NULL;
    char *filename = NULL;
    
    int argv_index = 1;
    if(argv[argv_index][0] == '-'){
        options = argv[argv_index];
        argv_index++;
    }
    pattern = argv[argv_index];
    argv_index++;

    if(argv_index < argc){
        filename = argv[argv_index];
    }

    
    int pattern_size = 0;
    for(int i = 0; pattern[i] != '\0'; i++){
        pattern_size++;
    }

    FILE *input_stream = stdin;

    if(filename != NULL){
        input_stream = fopen(filename,"r");
    }
    

    int return_value = 1;
    
    do{
        int line_size = 0;
        char* line = getline(&line_size,input_stream);
        
        int start_index = 0, start_color = 0, end_color = 0;
        int print_this_line = 0;
        
        for(int i = 0; i < line_size; i++){
            if(line[i] == pattern[0]){
                if(check_word(line, i, pattern)){
                    print_this_line = 1;
                    return_value = 0;
                    start_color = i;
                    end_color = i + pattern_size;
                    if(options != NULL && string_compare(options,COLOR_OPTION)){
                        print_colored_line(line, start_index,start_color,end_color);
                    }
                    else{
                        printf("%s\n", line);
                        break;
                    }
                    

                    start_index = end_color;
                    i = end_color;
                }
            }
        }
        
        if(options != NULL && string_compare(options,COLOR_OPTION) &&  print_this_line == 1){
            print_colored_line(line, start_index,line_size,line_size);
            printf("\n");
        }

        free(line);

    }while (!feof(input_stream));
    

    fclose(input_stream);    

    return return_value;
}
