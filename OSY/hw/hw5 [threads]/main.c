#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_N 100

typedef struct
{
    int num;
    char *text;
}data_pair;

typedef struct
{
    data_pair *data;
    struct linked_list *next;

} linked_list;

linked_list *list = NULL; // Create linked list root
linked_list *last_element = NULL; //Create linked list last_element

pthread_mutex_t mutex_data; // Mutex for accessing to data
sem_t sem_full; // Semaphore for checking is something in list

int is_producent_work = 1; // 1 - producent is working, 0 - producent isn't working


void append_element(int num, char *text){
    // create data instance
    data_pair *data = (data_pair*)malloc(sizeof(data_pair));
    data->num = num;
    data->text = text;

    // create linked list element
    linked_list *new_element = (linked_list*)malloc(sizeof(linked_list));
    new_element->data = data;
    new_element->next = NULL;

    
    if(list == NULL){
        // Save data to the root
        list = new_element;
    }
    else{
        // Link new_element to last element
        last_element->next = (struct linked_list *)new_element;
        
    }
    
    // Chane pointer from previous to last element
    last_element = new_element;
}

void free_element(linked_list *element){
    free((element->data)->text);
    free(element->data);
    free(element);
}

void *producent_work(){
    int *return_value = malloc(sizeof(int));
    *return_value = EXIT_SUCCESS;
    int ret, num;
    char *text;

    // Scan data and append to list
    while ((ret = scanf("%d %ms", &num, &text)) == 2) {
        if(num < 0){
            ret = 1;
            break;
        }

        // Lock mutex_data for accessing to data by only one thread
        pthread_mutex_lock(&mutex_data); 

        append_element(num,text);

        // Unlock mutex_data
        pthread_mutex_unlock(&mutex_data); 

        // Increment the semaphore (for start on consumer)
        sem_post(&sem_full);
    }

    // mutex lock, because of writing into is_producent_work
    pthread_mutex_lock(&mutex_data); 
    is_producent_work = 0;
    pthread_mutex_unlock(&mutex_data); 

    sem_post(&sem_full);

    // Check is scanf ended with success
    if(ret != 2 && ret != EOF){  
        *return_value = EXIT_FAILURE;
    }

    
    return (void*)return_value;
}

void *routine(void *number){
    int int_num =*((int*)number);

    while (1)
    {   
        // mutex lock, because of reading is_producent_work and list
        pthread_mutex_lock(&mutex_data); 
        if(!is_producent_work && !list){
            pthread_mutex_unlock(&mutex_data); 
            break;
        }
        pthread_mutex_unlock(&mutex_data); 

        // Semaphore for starting work, when element was added
        sem_wait(&sem_full);

        // Lock mutex_data for accessing to data by only one thread
        pthread_mutex_lock(&mutex_data);
        
        // Get data
        linked_list *current_element = list;
        if(current_element){
            list = (linked_list*)list->next;
        }
        else{
            // Unlock mutex_data
            pthread_mutex_unlock(&mutex_data);
            sem_post(&sem_full);
            continue;
        }

        // Unlock mutex_data
        pthread_mutex_unlock(&mutex_data);

        // Lock stdout for printing by only one thread
        flockfile(stdout);

        // Print data
        printf("Thread %d:",int_num);
        for(int i = 0; i < current_element->data->num; i++){
            printf(" %s",current_element->data->text);
        }
        printf("\n");

        // Unlock stdout
        funlockfile(stdout);
        
        // Free alocated data
        free_element(current_element);

        // mutex lock, because of reading is_producent_work
        pthread_mutex_lock(&mutex_data); 
        if(!is_producent_work){
            sem_post(&sem_full);
        }
        pthread_mutex_unlock(&mutex_data); 
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int N = 1;

    // Check N from arguments
    if(argc > 1){
        N = atoi(argv[1]);
    }
    
    // Check if N less then max count of threads
    if(N > sysconf(_SC_NPROCESSORS_ONLN) || N == 0){
        return EXIT_FAILURE;
    }
    
    pthread_t producent;
    pthread_t consumers[MAX_N];
    int* numbers[MAX_N];

    int return_value = EXIT_SUCCESS;
    int *return_value_producent;

    // Init mutex_data
    pthread_mutex_init(&mutex_data, NULL);

    // Init semaphore for accessing data when they are
    sem_init(&sem_full,0,0);

    //create producent
    if(pthread_create(&producent,NULL, &producent_work, NULL) != 0){
        return EXIT_FAILURE;
    }

    // create cosumers
    for(int i = 0; i < N; i++){
        numbers[i] = malloc(sizeof(int));
        *numbers[i] = i+1;
        if(pthread_create(&consumers[i],NULL, &routine, numbers[i]) != 0){
            return EXIT_FAILURE;
        }
    }

    // join producent
    if(pthread_join(producent,(void**)&return_value_producent) != 0){
        return EXIT_FAILURE;
    }

    //join consumers
    for(int i = 0 ; i < N; i++){
        if( pthread_join(consumers[i],NULL) != 0){
            return EXIT_FAILURE;
        }
    }   

    return_value+= *return_value_producent;
    
    free(return_value_producent);
    // Free allocated thread numbers
    for(int i = 0; i < N; i++){
        free(numbers[i]);
    }
    
    // Destroy mutexes and semaphores
    pthread_mutex_destroy(&mutex_data);
    sem_destroy(&sem_full);

    return return_value;
}
