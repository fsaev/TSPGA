#include <math.h>
#include <stdio.h>
#include "graph_tb.h"
#include "graph.h"

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

  link_nodes(&n1, &n2, 45.1);
  link_nodes(&n2, &n3, 11.2);

  np = &n1;

  while(np->link_count){
    sum += np->links[0]->distance;
    np = np->links[0]->link;
  }

  printf("Sum is %f\n", sum);

  if(sum == 56.3){
    return 0;
  }else{
    return 1;
  }
}