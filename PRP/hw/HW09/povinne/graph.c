#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

/* This is a testing file just to be able to compile the 
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

// - new graph allokating 
graph_t* allocate_graph() 
{
   graph_t* graph = malloc(sizeof(graph_t));
   graph->capacity = INIT_CAP;
   graph->size = 0;
   for(int i = 0; i < 3; i++){
      graph->matrix[i] = malloc(INIT_CAP * sizeof(int));
   }
   return graph;
}

void realloc_graph(graph_t *graph){
   for(int i = 0; i < 3; i++){
      int* new_array = realloc(graph->matrix[i], 2 * graph->capacity * sizeof(int));
      graph->matrix[i] = new_array;
   }
   graph->capacity*= 2;
}

// - free allocated memory
void free_graph(graph_t **graph)
{
   
   for(int i = 0; i < 3; i++){
      free((*graph)->matrix[i]);
   }
   free(*graph);
}

// - make txt with graph data
void load_txt(const char *fname, graph_t *graph)
{
   
   FILE *input_file = fopen(fname, "r");
   int i = 0;
   int ret1 = 3;
   while (!feof(input_file) && ret1 == 3){
      ret1 = fscanf(input_file,"%d %d %d\n",&graph->matrix[0][i], &graph->matrix[1][i],&graph->matrix[2][i]);
      graph->size++;

      if(graph->size == graph->capacity){
         realloc_graph(graph);
      }

      i++;
   }
   
   fclose(input_file);
   
}

// - load graph from .bin file
void load_bin(const char *fname, graph_t *graph)
{
   FILE *input_file = fopen(fname, "rb");
   int i = 0;
   while(fread(&graph->matrix[0][i],sizeof(int),1,input_file) && fread(&graph->matrix[1][i],sizeof(int),1,input_file) && fread(&graph->matrix[2][i],sizeof(int),1,input_file))
   {
      graph->size++;
      if(graph->size == graph->capacity){
         realloc_graph(graph);
      }
      i++;
   }
   
   fclose(input_file);
}

// - save graph to .txt file
void save_txt(const graph_t * const graph, const char *fname)
{
   FILE *output_file = fopen(fname,"w");
   for(int i = 0; i < graph->size; i++){
      fprintf(output_file,"%d %d %d\n",graph->matrix[0][i], graph->matrix[1][i],graph->matrix[2][i]);
   }
   fclose(output_file);

}

// - save file to .bin file
void save_bin(const graph_t * const graph, const char *fname)
{
   FILE *output_file = fopen(fname,"wb");
   for(int i = 0; i < graph->size; i++){
      fwrite(&graph->matrix[0][i],sizeof(int),1,output_file);
      fwrite(&graph->matrix[1][i],sizeof(int),1,output_file);
      fwrite(&graph->matrix[2][i],sizeof(int),1,output_file);
   }
   fclose(output_file);
   
}
