#include <stdlib.h>
#include <string.h>
#include "tsp.h"
#include "graph.h"

node_t **nodes = 0;
int node_count = 0;

int create_graph_nodes(char **data, int data_len)
{
    if(nodes){ return 1; }
    
    nodes = malloc(sizeof(node_t *) * data_len); //Allocat space for nodes

    for(int i = 0; i < data_len; i++){
        nodes[i] = malloc(sizeof(node_t)); //Allocate node
        nodes[i]->data = data[i]; //Link content
        nodes[i]->data_len = strlen(data[i]);
    }
}

int link_graph_node(int index, char **data, int data_len)
{
    node_t* n = nodes[index];
    n->links = malloc(sizeof(nvertex_t *) * data_len);

    for(int i = 0; i < data_len; i++){
        nvertex_t* v = malloc(sizeof(nvertex_t));
        v->link = nodes[i]; //
        v->distance = atof(data[i]);
        n->links[i] = v;
        printf("Linked %s to %s with distance %2f\n\r", (char *) n->data, v->link->data, v->distance);
    }
}