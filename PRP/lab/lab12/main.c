#include <stdio.h>

#include "binary_heap.h"

int main(int argc, char const *argv[])
{
    binary_heap* heap = create_binary_heap();
    if (heap == NULL) {
        return -1;
    }

    printf("Init heap: size %d, capacity %d\n", heap->size, heap->capacity);

    return 0;
}
