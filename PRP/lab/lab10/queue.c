#include<stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 100

typedef struct
{
    int *arr;
    int tail;
    int capacity;
    int head;
} Queue;


int is_empty(Queue *q){
    return (q->tail - q->head) == 0;
}

void push(Queue *q, int num){
    if(q->tail == q->capacity){
        fprintf(stderr, "Memory error\n");
        exit(100);
    }
    q->arr[q->tail] = num;
    (q->tail)++;
}

int pop(Queue *q){
    if(is_empty(q)){
        fprintf(stderr,"Error: Queue is empty\n");
        return -1;
    }
    int poped_element = q->arr[q->head-1];
    (q->head)++;
    q->arr[q->head-1] = 0;
    return poped_element;
}



int head(Queue *q){
    if(is_empty(q)){
        fprintf(stderr,"Error: Queue is empty\n");
        return -1;
    }
    return q->arr[q->head];
}

void print_queue(Queue *q){
    for(int i = q->head; i < q->tail; i++){
        printf("%d ",q->arr[i]);
    }
    printf("\n");
}

int main(){
    /*Queue init */
    Queue *q = (Queue*) malloc (sizeof(Queue) * 1);
    if (!q){
        fprintf(stderr, "Allocation failed!\n");
        return 100;
    }

    q->capacity = INIT_CAPACITY;
    q-> arr = (int*)malloc(sizeof(int) * q->capacity);
    if(!q->arr){
        free(q);
        fprintf(stderr, "Allocation failed!\n");
        return 100;
    }
    q->head = 0;
    q->head = 0;

    

    /*testing code*/
    for(int i = 0; i < 20; i++){
        push(q,i*2);
    }
    
    print_queue(q);
    for(int i = 0 ; i < 5; i++){
        pop(q);
    }
    print_queue(q);


    //FREE queue
    q->head = 0;
    q->tail = 0;
    free(q->arr);
    free(q);
    
}