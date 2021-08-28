#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_tb.h"
#include "reader_tb.h"

int main(int argc, char **argv)
{
  int code = 0;
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

  } else if(!strcmp(argv[1], "CSVREADER")){
    printf("CSVREADER test\n");

    if((code = reader_read_test()) == 0){
      printf("SUCCESS\n");
    }else{
      printf("FAIL %d\n", code);
    }
  }

}