#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_tb.h"
#include "reader_tb.h"
#include "reader.h"
#include "phenotype.h"

int main(int argc, char **argv)
{
  int code = 0;
  char *g_entries[30];

  printf("Running Testbench\n");

  if (argc < 2) {
    printf("usage: test <argument>\n");
    printf("    If <argument> is 0, print SUCCESS. Otherwise print FAIL.\n");
    exit(1);
  }

  if(!strcmp(argv[1], "GRAPHLINK")){
    printf("Graphlink test\n");

    if(graph_init_test()){
      printf("INITFAIL");
      exit(1);
    }

    if((code = graph_link_test()) == 0){
      printf("SUCCESS\n");
    }else{
      printf("FAIL %d\n", code);
    }
  } else if(!strcmp(argv[1], "GRAPHTRAVERSE")){
    printf("Graphtraverse test\n");

    graph_t *tsp_graph = malloc(sizeof(graph_t));

    if(open_csv("../assets/european_cities.csv")){
      printf("INITFAIL");
      exit(1);
    }

    /*
     * Read Header
     */
    int len = line_csv(g_entries, 30);
    if(len){
        create_graph_nodes(tsp_graph, g_entries, len);
    }

    /*
     * Read Data
     */
    int i = 0;
    while((len = line_csv(g_entries, 30))){
        if(len){
            link_graph_node(tsp_graph, i++, g_entries, len);
        }
    }

    if((code = graph_test_traverse(tsp_graph)) == 0){
      printf("SUCCESS\n");
    }else{
      printf("FAIL %d\n", code);
    }

  } else if(!strcmp(argv[1], "CSVREADER")){
    printf("CSVREADER test\n");

    if((code = reader_read_test()) == 0){
      printf("SUCCESS\n");
    }else{
      printf("FAIL %d\n", code);
    }
  } else if(!strcmp(argv[1], "PERMUTEGENOME")){
    int sum = 0;
    genome_t* g = malloc(sizeof(genome_t));
    int tseq[24];

    for(int i = 0; i < 24; i++){
      tseq[i] = i;
    }
    g->sequence = tseq;
    g->length = 23;

    if((g->sequence[12] == 12) && (g->sequence[23] == 23)){
        for(int i = 0; i < 10000; i++){
            permute(g->sequence, g->length);
        }
        for(int i = 0; i < 24; i++){
            sum += tseq[i];
        }
        /* If sum matches triangle number of 23 */
        if(sum == (23 * ((23 + 1) / 2))){
            printf("SUCCESS\n");
        }else{
            printf("FAIL permute\n");
        }
    }else{
      printf("FAIL genome\n");
    }
  }

}
