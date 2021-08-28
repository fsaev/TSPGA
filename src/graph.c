#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


#ifdef TESTBENCH
int graph_run_test(void){
    return 0;
}
#endif

node_t create_node(void *data, int data_len)
{
    node_t n;

    n.data = data;
    n.data_len = data_len;
    n.links = 0;
    n.link_count = 0;

    return n;
}

int link_nodes(node_t* from, node_t* to, float distance)
{
        nvertex_t* v = malloc(sizeof(nvertex_t));
        v->link = to; //
        v->distance = distance;
        from->links[from->link_count++] = v;
        printf("Linked %s to %s with distance %2f\n\r", (char *) from->data, (char *) v->link->data, v->distance);
        return 0;
}