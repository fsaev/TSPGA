#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tsp.h"


node_t **nodes = 0;
int node_count = 0;

float best_fitness = 0;

static void print_solution(int *x, int len)
{
    float total_distance = 0;
    node_t* cur = nodes[x[0]]; //Get first node

    printf("NEW BEST\n\r");
    cur = nodes[x[0]]; //Get first node
    for(int i = 1; i < len; i++){
        nvertex_t* v = cur->links[x[i]]; //Get vertex from genome
        total_distance += v->distance;
        printf("%s -> ", (char *) cur->data);
        cur = v->link; //Get next node
    }

    printf("END | Total: %0f\n\r", total_distance);
}

int measure_fitness(int *x, int len)
{
    float total_distance = 0;
    node_t* cur = nodes[x[0]]; //Get first node
    for(int i = 1; i < len; i++){
        nvertex_t* v = cur->links[x[i]]; //Get vertex from genome
        total_distance += v->distance;
        cur = v->link; //Get next node
    }

    if(!best_fitness || best_fitness > total_distance){
        best_fitness = total_distance;
        print_solution(x, len);
    }
    return 0;
}

int create_graph_nodes(char **data, int data_len)
{
    if(nodes){ return 1; }
    
    nodes = malloc(sizeof(node_t *) * data_len); //Allocate space for nodes
    node_count = data_len;

    for(int i = 0; i < data_len; i++){
        nodes[i] = malloc(sizeof(node_t)); //Allocate node
        nodes[i]->data = data[i]; //Link content
        nodes[i]->data_len = strlen(data[i]);
    }
    return 0;
}

int link_graph_node(int index, char **data, int data_len)
{
    node_t* n = nodes[index];
    n->links = malloc(sizeof(nvertex_t *) * data_len);

    for(int i = 0; i < data_len; i++){
        link_nodes(n, nodes[i], atof(data[i]));
    }
    return 0;
}

int get_node_count(void)
{
    return node_count;
}

node_t** get_nodes(void)
{
    return nodes;
}
