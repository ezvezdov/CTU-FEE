#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include <assert.h>

#include "dijkstra.h"
#include "graph.h"
#include "graph_utils.h"
#include "my_malloc.h"

#include "pq_heap.h"



typedef struct {
   int edge_start; // index to the first edge in the array of edges (-1 if does not exist)
   int edge_count; // number of edges (may be 0)
   int parent; // index to the parent node on the shortest path from the given node 
   int cost; // cost of the shortest path from the starting node to this node 
} node_t;

typedef struct {
   graph_t *graph;
   node_t *nodes;
   int num_nodes; // number of nodes;
   int start_node; //
} dijkstra_t;

// - function ----------------------------------------------------------------
void* dijkstra_init(void)
{
   dijkstra_t *dij = myMalloc(sizeof(dijkstra_t));
   dij->nodes = NULL;
   dij->num_nodes = 0;
   dij->start_node = -1;
   dij->graph = allocate_graph();
   if (!dij->graph) {
      free(dij); 
      dij = NULL;
   }
   return dij;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_load_graph(const char *filename, void *dijkstra)
{
   _Bool ret = false;
   dijkstra_t *dij = (dijkstra_t*)dijkstra;
   if (
         dij && dij->graph
      ) { // edges has not been loaded
      // dijkstra_t and graph has been allocated and edges have been loaded here
      // go through the edges and create array of nodes with indexing to edges
      // 1st get the maximal number of nodes
      
      load_txt(filename,dij->graph);

      int m = dij->graph->num_nodes;

      m += 1; // m is the index therefore we need +1 for label 0
      dij->nodes = myMalloc(sizeof(node_t) * m);

      dij->num_nodes = m;

      // 2nd initialization of the nodes
      for (int i = 0; i < m; ++i) {
         dij->nodes[i].edge_start = -1;
         dij->nodes[i].edge_count = 0;
         dij->nodes[i].parent = -1;
         dij->nodes[i].cost = -1;
      }

      // 3nd add edges to the nodes
      for (int i = 0; i < dij->graph->num_edges; ++i) {
         int cur = dij->graph->edges[i].from;
         if (dij->nodes[cur].edge_start == -1) { // first edge
            dij->nodes[cur].edge_start = i; // mark the first edge in the array of edges
         }
         dij->nodes[cur].edge_count += 1; // increase number of edges
      }
      ret = true;
   }
   return ret;
}


/*
 * Set the graph to the dijkstra structure instead of direct 
 * loading the file as in dijkstra_load_graph() function.
 * The given array edges should not be directly used for the
 * internal representation of the graph in the dijkstra algorithm
 *
 * e - number of edges, i.e., size of the array 
 * 
 * return: true on success; false on an error, e.g., memory allocation
 */
_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra){
   _Bool ret = true;
   dijkstra_t *dij = (dijkstra_t*)dijkstra; 

   while (dij->graph->capacity < e)
   {
      enlarge_graph(dij->graph);
   }
   dij->graph->num_edges = e;
   
   for(int i = 0; i < e; i++){
      dij->graph->edges[i].from = edges[i][0];
      dij->graph->edges[i].to = edges[i][1];
      dij->graph->edges[i].cost = edges[i][2];
   }
   
   

   int m = -1;
   for (int i = 0; i < dij->graph->num_edges; ++i) {
      const edge_t *const e = &(dij->graph->edges[i]); // use pointer to avoid copying
      m = m < e->from ? e->from : m;
      m = m < e->to ? e->to : m;
   }
   m += 1; // m is the index therefore we need +1 for label 0
   dij->nodes = myMalloc(sizeof(node_t) * m);

   dij->num_nodes = m;

   // 2nd initialization of the nodes
   for (int i = 0; i < m; ++i) {
      dij->nodes[i].edge_start = -1;
      dij->nodes[i].edge_count = 0;
      dij->nodes[i].parent = -1;
      dij->nodes[i].cost = -1;
   }

      // 3nd add edges to the nodes
   for (int i = 0; i < dij->graph->num_edges; ++i) {
      int cur = dij->graph->edges[i].from;
      if (dij->nodes[cur].edge_start == -1) { // first edge
         dij->nodes[cur].edge_start = i; // mark the first edge in the array of edges
      }
      dij->nodes[cur].edge_count += 1; // increase number of edges
   }
   ret = true;
   return ret;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_solve(void *dijkstra, int label) 
{
   dijkstra_t *dij = (dijkstra_t*)dijkstra;
   if (!dij || label < 0 || label >= dij->num_nodes) {
      return false;
   }
   dij->start_node = label;

   void *pq = pq_alloc(dij->num_nodes);

   dij->nodes[label].cost = 0; // initialize the starting node
   pq_push(pq, label, 0);

   int cur_label;
   while ( !pq_is_empty(pq) && pq_pop(pq, &cur_label) ) {
      node_t *cur = &(dij->nodes[cur_label]);
      for (int i = 0; i < cur->edge_count; ++i) { // relax all children 
         edge_t *edge = &(dij->graph->edges[cur->edge_start + i]); // avoid copying
         node_t *to = &(dij->nodes[edge->to]);
         const int cost = cur->cost + edge->cost;
         
         if (to->cost == -1) { // the node to has not been visited yet
            to->cost = cost;
            to->parent = cur_label;
            pq_push(pq, edge->to, cost); 
         } else if (cost < to->cost) { // already relaxed check if we can make a shortcut to child node via the current node
            to->cost = cost;
            to->parent = cur_label;
            pq_update(pq, edge->to, cost);
         }
      } // end all children of the cur node;
   } // end pq_is_empty

   pq_free(pq); // release the memory

   return true;
}

_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3]){
   _Bool ret = true;
   const dijkstra_t *const dij = (dijkstra_t*)dijkstra;
   for (int i = 0; i <dij->num_nodes; ++i) {
      const node_t *const node = &(dij->nodes[i]);
      solution[i][0] = i;
      solution[i][1] = node->cost;
      solution[i][2] = node->parent;
   } // end all nodes
   return ret;  
}

// - function ----------------------------------------------------------------
_Bool dijkstra_save_path(const void *dijkstra, const char *filename)
{
   _Bool ret = false;

   const dijkstra_t *const dij = (dijkstra_t*)dijkstra;
   if (dij) {

      FILE *f = fopen(filename, "w");
      if (f) {
         for (int i = 0; i <dij->num_nodes; ++i) {
            const node_t *const node = &(dij->nodes[i]);
               fprintf(f, "%i %i %i\n", i, node->cost, node->parent);     
         } // end all nodes
         ret = fclose(f) == 0;
      }
   }
   return ret;
}

// - function ----------------------------------------------------------------
void dijkstra_free(void *dijkstra)
{
   dijkstra_t *dij = (dijkstra_t*)dijkstra;
   if (dij) {
      if (dij->graph) {
         free_graph(&(dij->graph));
      }
      if (dij->nodes) {
         free(dij->nodes);
      }
      free(dij);
   }
}

/* end of dijkstra.c */
