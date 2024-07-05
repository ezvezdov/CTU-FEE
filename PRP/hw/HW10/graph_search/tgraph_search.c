#include "dijkstra.h"
#include "graph.h"
#include "graph_utils.h"

#include <stdio.h>

int main(int argc, char *argv[]) 
{
   int ret = 0;

   if (argc < 3) {
      fprintf(stderr, "Call as\n %s graph_file solution_file\n", argv[0]);
   } else {
      graph_t *g = NULL;
      g =  allocate_graph(g);
      load_txt(argv[1],g);
      save_bin(g,argv[2]);
      load_bin(argv[2],g);
      /*
      fprintf(stderr, "Load graph from %s\n", argv[1]);
      void *dij1 = dijkstra_init();


      dijkstra_load_graph(argv[1], dij1);
      fprintf(stderr, "Find all shortest paths from the node 0\n");
      dijkstra_solve(dij1, 0);
      fprintf(stderr, "Save solution to %s\n", argv[2]);
      dijkstra_save_path(dij1, argv[2]);
      fprintf(stderr, "Free allocated memory\n");
      dijkstra_free(dij1);
      */
      ret = 0;
   }
   return ret;
}

/* end of tgraph_search.c */
