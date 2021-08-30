#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include "graph.h"
#include "phenotype.h"

typedef struct{
  int wid;

  int genome_sequence[24];
  genome_t genome;
} worker_t;

void start_bruteforce();

#endif
