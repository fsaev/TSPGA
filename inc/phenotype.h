#ifndef PHENOTYPE_H
#define PHENOTYPE_H

typedef struct{
  int *sequence;
  int length;
} genome_t;

int permute(int *a, int n);
void fisheryates_shuffle(int *array, int n, int passes);

#endif
