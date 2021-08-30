#include <string.h>
#include <stdio.h>
#include "reader_tb.h"
#include "reader.h"

int reader_read_test(void){
  if(open_csv("../assets/european_cities.csv")){
    return 1;
  }
  int len = 0;
  char **entries = line_csv(&len);

  if(!strcmp("Barcelona", entries[0]) &&
      !strcmp("Warsaw", entries[23])){
        printf("Found %s at first element and %s at last\n", entries[0], entries[23]);
        return 0;
  }

  return 1;
}