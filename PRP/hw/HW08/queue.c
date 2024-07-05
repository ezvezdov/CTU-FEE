#include "queue.h"
 
/* creates a new queue with a given size */
queue_t* create_queue(int capacity){
    queue_t *queue = malloc(sizeof(queue_t));
    queue->capacity = capacity;
    queue->size = 0;
    queue->array = malloc(capacity * sizeof(void*));
    queue->end = queue->array;
    queue->start = queue->array;
 
    return queue;
}
 
void realloc_queue(queue_t *queue, int new_capaity){
    //printf("OLD SIZE: %d\nNEW SIZE: %d\n",queue->capacity, new_capaity);
    void** new_arr = malloc(new_capaity * sizeof(void*));
    for(int i = 0; i < queue->size; i++){
        if( (queue->start + i) >= (queue->array + queue->capacity) ){
            new_arr[i] = queue->array[i - ((queue->array + queue->capacity) - queue->start)];
        }
        else{
            new_arr[i] = queue->start[i];
        }
    }
    free(queue->array);
 
    queue->capacity = new_capaity;
    queue->array = new_arr;
    queue->start = queue->array;
    queue->end = queue->array + queue->size;
}
 
 
/* deletes the queue and all allocated memory */
void delete_queue(queue_t *queue){
    free(queue->array);
    free(queue);
}
 
/*
 * inserts a reference to the element into the queue
 * returns: true on success; false otherwise
 */
bool push_to_queue(queue_t *queue, void *data){
    if(queue->size == queue->capacity){
        realloc_queue(queue,queue->capacity*2);
    }
    
    *queue->end = data;
 
    if(queue->end == (queue->array + (queue->capacity-1)) ){
        queue->end = queue->array;
    }
    else{
        queue->end++;    
    }

    
     
 
    queue->size++;
    return true;
 
}
 
 
/*
 * gets the first element from the queue and removes it from the queue
 * returns: the first element on success; NULL otherwise
 */
void* pop_from_queue(queue_t *queue){
    if(queue->size == 0){ 
        return NULL;
    }
 
    if(queue->size < ((2 *queue->capacity)/3) && queue->capacity > 5){
        realloc_queue(queue,(2 *queue->capacity)/3);
    }
 
    void* ret = *queue->start;
    if(queue->start+1 >= (queue->array + queue->capacity)){
        queue->start = queue->array;
    }
    else{
        queue->start++;
    }
    queue->size--;
    return ret;
}
 
/*
 * gets idx-th element from the queue, i.e., it returns the element that 
 * will be popped after idx calls of the pop_from_queue()
 * returns: the idx-th element on success; NULL otherwise
 */
void* get_from_queue(queue_t *queue, int idx){
    if(idx >= queue->size || idx < 0){
        return NULL;
    };
 
    void** ret;
    if( (queue->start + idx) >= (queue->array + queue->capacity) ){
        idx-= ((queue->array + queue->capacity) - queue->start);
        ret = queue->array+idx;
    }
    else{
        ret = queue->start+idx;
    }
    return *ret;
 
 
}
 
/* gets number of stored elements */
int get_queue_size(queue_t *queue){
    return queue->size;
}
