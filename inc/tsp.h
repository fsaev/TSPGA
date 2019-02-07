#ifndef TSP_H
#define TSP_H

#include "graph.h"

int measure_fitness(int *x, int len);
int create_graph_nodes(char **data, int data_len);
int link_graph_node(int index, char **data, int data_len);
int get_node_count(void);
node_t** get_nodes(void);


#endif