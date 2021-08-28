#ifndef GRAPH_H
#define GRAPH_H

typedef struct nvertex nvertex_t;
typedef struct node node_t;

struct nvertex{
    node_t* link;
    float distance;
};

struct node{
    void *data;
    int data_len;

    nvertex_t** links;
    int link_count;
};


#ifdef TESTBENCH
int graph_run_test();
#endif

node_t create_node(void *data, int data_len);
int link_nodes(node_t* from, node_t* to, float distance);

#endif