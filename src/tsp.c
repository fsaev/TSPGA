#include <stdio.h>
#include "tsp.h"

float evaluate(genome_t *g, graph_t *gr){
  float sum = 0;

  if(!g->length){ return 0; }

  node_t *n = gr->nodes[g->sequence[0]]; //Get start position of sequence
  //printf("[%s]", (char *) n->data);
  for(int i = 1; i < g->length; i++){
    //Get distance to next in sequence
    sum += n->links[g->sequence[i]]->distance;
    //printf(" -(%.2f)-> ", sum);
    //Move to next node in sequence
    n = n->links[g->sequence[i]]->link;
    //if(n){ printf("[%s]", (char *) n->data); }
  }
  return sum;
}
