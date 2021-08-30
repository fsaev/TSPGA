#ifndef PHENOTYPE_H
#define PHENOTYPE_H

typedef struct{
  int *sequence;
  int length;
} genome_t;

int permute(int *a, int n);
void shuffle(genome_t *g, int passes);

#endif
