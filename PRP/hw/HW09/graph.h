#ifndef __GRAPH_H__
#define __GRAPH_H__

#define INIT_CAP 100

typedef struct {
   int* matrix[3]; // graph presentation
   size_t size;
   size_t capacity; // capacity of graph
} graph_t;

/* Allocate a new graph and return a reference to it. */
graph_t* allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);
/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname);
/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname);

#endif // __GRAPH_H__
