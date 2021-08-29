#ifndef GRAPH_H
#define GRAPH_H

typedef struct nvertex nvertex_t;
typedef struct node node_t;
typedef struct graph graph_t;

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

struct graph{
    node_t **nodes;
    int count;
};


node_t create_node(void *data, int data_len);
int link_nodes(node_t *from, node_t *to, float distance);
void create_graph_nodes(graph_t* graph, char **data, int data_len);
int link_graph_node(graph_t *graph, int index, char **data, int data_len);

#endif
