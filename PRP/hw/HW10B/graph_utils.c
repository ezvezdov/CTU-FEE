#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "graph.h"
#include "my_malloc.h"

void print_graph(graph_t *g) 
{
   assert(g != NULL);
   fprintf(stderr, "Graph has %d edges and %d edges are allocated\n", g->num_edges, g->capacity);
   edge_t *e = g->edges;
   for (int i = 0; i < g->num_edges; ++i, ++e) {
      printf("%d %d %d\n", e->from, e->to, e->cost);
   }
   printf("\n");
}

int str_to_int(char *str,int index){
   int num = 0;
   int base = 1;
   for(int i = index-1; i >= 0; i--){
      num += (str[i] - '0') * base;
      base*=10;
   }
   return num;
}
