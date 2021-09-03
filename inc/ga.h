#ifndef GA_H
#define GA_H

#include "graph.h"
#include "phenotype.h"

void pmx_crossover(genome_t *p1, genome_t *p2, genome_t *o1);
int start_ga(int threads, int start_population);

#endif
