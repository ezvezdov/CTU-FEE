#include "binary_heap.h"    

binary_heap* create_binary_heap() {
    binary_heap *b_heap = (binary_heap*) malloc (sizeof(binary_heap));
    if (!b_heap) {
        fprintf(stderr, "Error while allocating the binary heap!\n");
        return NULL;
    }

    b_heap->capacity = INIT_CAP;
    b_heap->size = 0;
    b_heap->data = (int*) malloc (sizeof(int) * b_heap->capacity);
    if (!b_heap->data) {
        free(b_heap);
        fprintf(stderr, "Error while allocating the binary heap!\n");
        return NULL;
    }

    return b_heap;
}
