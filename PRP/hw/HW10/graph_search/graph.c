#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>

#include "graph.h"
#include "my_malloc.h"
#include "graph_utils.h"


#ifndef INIT_SIZE 
#define INIT_SIZE 50000000
#endif

// - new graph allokating
graph_t* allocate_graph(void) 
{
   graph_t *g = myMalloc(sizeof(graph_t));
   g->edges = NULL;
   g->num_edges = 0;
   g->capacity = 0;
   g->num_nodes = 0;
   /* or we can call calloc */
   return g;
}

void free_graph(graph_t **g) 
{
   assert(g != NULL && *g != NULL);
   if ((*g)->capacity > 0) {
      free((*g)->edges);
   }
   free(*g);
   *g = NULL;
}

graph_t* enlarge_graph(graph_t *g) 
{
   assert(g != NULL);
   int n = g->capacity == 0 ? INIT_SIZE : g->capacity * 2; /* double the memory */

   edge_t *e = myMalloc(n * sizeof(edge_t));
   memcpy(e, g->edges, g->num_edges * sizeof(edge_t));
   free(g->edges);
   g->edges = e;
   g->capacity = n;
   return g;
}


// - make txt with graph data
void load_txt(const char *fname, graph_t *graph)
{
   
    int exit = 0;
    FILE *f = fopen(fname, "r");
    while (f && !exit) {
        if (graph->num_edges == graph->capacity) {
            enlarge_graph(graph);
        }
        edge_t *e = graph->edges + graph->num_edges;
        while (graph->num_edges < graph->capacity) {
            char ch = '0';
            int r = 0;

            while (ch != '\n' && ch != EOF){
                char int_str[10] = "";
                int index = 0;

                ch = fgetc(f);
                while (ch != ' ' && ch != '\n' && ch != EOF){
                    int_str[index] = ch;
                    index++;
                    ch = fgetc(f);
                }
                if(r == 0){
                    e->from = str_to_int(int_str,index);
                    if(e->from > graph->num_nodes){
                        graph->num_nodes = e->from;
                    }
                }
                if(r == 1){
                    e->to = str_to_int(int_str,index);
                    if(e->to > graph->num_nodes){
                        graph->num_nodes = e->to;
                    }
                }
                if(r == 2){
                    e->cost = str_to_int(int_str,index);
                }
                r++;
            }
            if (r == 3) {
                graph->num_edges += 1;
                e += 1;
            }  
            else {
                exit = 1;
                break;
            }
        }
    }
    if (f) {
        fclose(f);
    }
}

// - load graph from .bin file
void load_bin(const char *fname, graph_t *graph)
{
    FILE *input_file = fopen(fname, "rb");
    int i = 0;
    if (graph->num_edges == graph->capacity) {
            enlarge_graph(graph);
    }
    uint32_t a = -1, b = -1, c = -1;
    while(fread(&a,sizeof(int),1,input_file) && fread(&b,sizeof(int),1,input_file) && fread(&c,sizeof(int),1,input_file))
    {   
        graph->edges[i].from = ntohl(a);
        graph->edges[i].to = ntohl(b);
        graph->edges[i].cost = ntohl(c);
        if(graph->edges[i].from > graph->num_nodes){
            graph->num_nodes = graph->edges[i].from;
        }
        if(graph->edges[i].to > graph->num_nodes){
            graph->num_nodes = graph->edges[i].to;
        }
        
        graph->num_edges++;
        if (graph->num_edges == graph->capacity) {
            enlarge_graph(graph);
        }
        i++;
    }
   
    fclose(input_file);
}

// - save graph to .txt file
void save_txt(const graph_t * const graph, const char *fname)
{
    FILE *output_file = fopen(fname,"w");
    for(int i = 0; i < graph->num_edges; i++){
        char snum[10] = "";
        for(int j = 0; j < 3; j++ ){
            
            if(j == 0){
                sprintf(snum, "%d", graph->edges[i].from);
            }
            else if(j == 1){
                sprintf(snum, "%d", graph->edges[i].to);
            }
            else if(j == 2){
                sprintf(snum, "%d", graph->edges[i].cost);
            }
            for(int k = 0; snum[k] != '\0';k++){
                fputc(snum[k],output_file);
            }
            if(j != 2){
                fputc(' ',output_file);
            }
        }
        fputc('\n',output_file);
    }
}

// - save file to .bin file
void save_bin(const graph_t * const graph, const char *fname)
{
    FILE *output_file = fopen(fname,"wb");
    for(int i = 0; i < graph->num_edges; i++){
        uint32_t a = htonl(graph->edges[i].from);
        uint32_t b = htonl(graph->edges[i].to);
        uint32_t c = htonl(graph->edges[i].cost);

        fwrite(&a,sizeof(int),1,output_file);
        fwrite(&b,sizeof(int),1,output_file);
        fwrite(&c,sizeof(int),1,output_file);
    }
    fclose(output_file);
   
}
