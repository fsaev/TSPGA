#include <math.h>
#include <stdio.h>
#include "graph_tb.h"

int test_data1 = 1234;
int test_data2 = 5678;
int test_data3 = 1337;

node_t n1, n2, n3;

int graph_init_test(void){
  n1 = create_node(&test_data1, sizeof(int));
  n2 = create_node(&test_data2, sizeof(int));
  n3 = create_node(&test_data3, sizeof(int));
  return 0;
}

int graph_link_test(void){

  float sum = 0;
  node_t *np;

  link_nodes(&n1, &n2, 45.0);
  link_nodes(&n2, &n3, 11.0);

  np = &n1;

  while(np->link_count){
    sum += np->links[0]->distance;
    np = np->links[0]->link;
  }

  if((int) sum == 56){
    return 0;
  }else{
    return 1;
  }
}

int graph_test_traverse(graph_t *graph){
    int sequence[] = {2, 3, 7, 4, 5, 8, 9, 14, 13};
    float sum = 0;

    node_t *n = graph->nodes[0];
    node_t *n_prev = n;
    nvertex_t *nlink = 0;

    for(unsigned int i = 0; i < sizeof(sequence)/sizeof(int); i++){
        nlink = n->links[sequence[i]];
        n = nlink->link;
        printf("Traveled %s -[%d]-> %s\n", (char *) n_prev->data, (int) nlink->distance, (char *) n->data);
        sum += nlink->distance;
        n_prev = n;
    }

    if((int) sum == 13045){
        return 0;
    }else{
        return 1;
    }
}