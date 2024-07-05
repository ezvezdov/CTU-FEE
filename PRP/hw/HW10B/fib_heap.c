#include <stdio.h>
#include <stdbool.h>

#include "my_malloc.h"

typedef struct _heap_node {
  int key;
  int degree;
  struct heap_node *left_sibling;
  struct heap_node *right_sibling;
  struct heap_node *parent;
  struct heap_node *child;
  bool mark;
  bool is_visited;
} heap_node;

typedef struct fibanocci_heap {
  int nodes_count;
  heap_node *min;
  int degree;
} heap;

heap *heap_alloc() {
  heap *h;
  h = (heap*)myMalloc(sizeof(heap));
  h->nodes_count = 0;
  h->min = NULL;
  h->degree = 0;
  return h;
}

// Inserting nodes
void insertion(heap *h, heap_node *new, int val) {
  new = (heap_node*)myMalloc(sizeof(heap_node));
  new->key = val;
  new->degree = 0;
  new->mark = false;
  new->parent = NULL;
  new->child = NULL;
  new->is_visited = false;
  new->left_sibling = new;
  new->right_sibling = new;
  if (h->min == NULL) {
    h->min = new;
  } else {
    h->min->left_sibling->right_sibling = new;
    new->right_sibling = h->min;
    new->left_sibling = h->min->left_sibling;
    h->min->left_sibling = new;
    if (new->key < h->min->key) {
      h->min = new;
    }
  }
  (h->nodes_count)++;
}

// Find min node
heap_node *min_node(heap *h) {
  if (h == NULL) {
    return NULL;
  } else
    return h->min;
}

// Union operation
heap *unionHeap(heap *h1, heap *h2) {
  heap *new_heap;
  new_heap = make_fib_heap();
  new_heap->min = h1->min;

  heap_node *tmp1, *tmp2;
  tmp1 = new_heap->min->right_sibling;
  tmp2 = h2->min->left_sibling;

  new_heap->min->right_sibling->left_sibling = h2->min->left_sibling;
  new_heap->min->right_sibling = h2->min;
  h2->min->left_sibling = new_heap->min;
  tmp2->right_sibling = tmp1;

  if ((h1->min == NULL) || (h2->min != NULL && h2->min->key < h1->min->key))
    new_heap->min = h2->min;
  new_heap->nodes_count = h1->nodes_count + h2->nodes_count;
  return new_heap;
}

