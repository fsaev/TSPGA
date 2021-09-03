#include <stdlib.h>
#include <stdio.h>
#include "ga_tb.h"
#include "ga.h"
#include "reader.h"

int ga_pmx_test(void){
  graph_t graph;
  genome_t *individuals;

  if(open_csv("../assets/european_cities.csv")){
    printf("INITFAIL");
    exit(1);
  }

  /*
  * Read Header
  */
  int len = 0;
  char **entries = line_csv(&len);
  if(len){
      create_graph_nodes(&graph, entries, len);
  }

  /*
  * Read Data
  */
  int i = 0;
  while((entries = line_csv(&len))){
      if(len){
          link_graph_node(&graph, i++, entries, len);
      }
  }

  individuals = malloc(sizeof(genome_t) * 2);

  for(int i = 0; i < 2; i++){
    individuals[i].sequence = malloc(sizeof(int) * 23);
    individuals[i].length = 23;
    for(int n = 0; n < 24; n++){
      individuals[i].sequence[n] = n;
    }
    fisheryates_shuffle(individuals[i].sequence, individuals[i].length, 8);
  }

  genome_t *o1 = malloc(sizeof(genome_t));

  o1->sequence = malloc(sizeof(int) * 23);
  o1->length = 23;
  pmx_crossover(&individuals[0], &individuals[1], o1);

  for(int m = 0; m < o1->length; m++){
    for(int n = 0; n < o1->length; n++){
      if(m != n){
        if((o1->sequence[m] == o1->sequence[n]) || (o1->sequence[m] == -1)){
          return 1;
        }
      }
    }
  }
  return 0;
}