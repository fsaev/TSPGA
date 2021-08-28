#include <string.h>
#include <stdio.h>
#include "reader_tb.h"
#include "reader.h"

int reader_read_test(void){
  char *g_entries[30];

  if(open_csv("../assets/european_cities.csv")){
    return 1;
  }

  line_csv(g_entries, 30);

  if(!strcmp("Barcelona", g_entries[0]) &&
      !strcmp("Warsaw", g_entries[23])){
        printf("Found %s at first element and %s at last\n", g_entries[0], g_entries[23]);
        return 0;
  }

  return 1;
}