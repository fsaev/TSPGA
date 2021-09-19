#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

node_t create_node(void *data, int data_len)
{
    node_t n;

    n.data = data;
    n.data_len = data_len;
    n.links = 0;
    n.link_count = 0;

    return n;
}

int link_nodes(node_t *from, node_t *to, float distance) {
        nvertex_t *v = malloc(sizeof(nvertex_t));
        v->link = to; //
        v->distance = distance;
        from->links = realloc(from->links, sizeof(nvertex_t*) * (from->link_count + 1));
        from->links[from->link_count++] = v;
        //printf("Linked %s to %s with distance %2f\n\r", (char *) from->data, (char *) v->link->data, v->distance);
        return 0;
}

void create_graph_nodes(graph_t *graph, char **data, int data_len) {
    graph->nodes = malloc(sizeof(node_t *) * data_len); //Allocate space for nodes
    graph->count = data_len;

    for(int i = 0; i < data_len; i++){
        graph->nodes[i] = malloc(sizeof(node_t)); //Allocate node
        graph->nodes[i]->data = data[i]; //Link content
        graph->nodes[i]->data_len = strlen(data[i]);
        graph->nodes[i]->links = 0;
        graph->nodes[i]->link_count = 0;
    }
}

int link_graph_node(graph_t *graph, int index, char **data, int data_len) {
    node_t *n = graph->nodes[index];

    for(int i = 0; i < data_len; i++){
        link_nodes(n, graph->nodes[i], atof(data[i]));
    }
    return 0;
}
