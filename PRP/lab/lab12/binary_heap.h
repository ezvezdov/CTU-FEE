#ifndef __BINARY_HEAP_H__
#define __BINARY_HEAP_H__


#include <stdio.h>
#include <stdlib.h>

#define INIT_CAP 10

typedef struct binary_heap
{
    int *data;
    int size;
    int capacity;
} binary_heap;

/* creates binary heap */
binary_heap* create_binary_heap();

#endif  /* __BINARY_HEAP_H__ */
