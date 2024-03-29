#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "phenotype.h"


/*
 * Copied from https://rosettacode.org/wiki/Permutations
 */

/* next lexicographical permutation */
int permute(int *a, int n) {
#define swap(i, j) {t = a[i]; a[i] = a[j]; a[j] = t;}
  int k, l, t;

  /* 1. Find the largest index k such that a[k] < a[k + 1]. If no such
        index exists, the permutation is the last permutation. */
  for (k = n - 1; k && a[k - 1] >= a[k]; k--);
  if (!k--) return 0;

  /* 2. Find the largest index l such that a[k] < a[l]. Since k + 1 is
     such an index, l is well defined */
  for (l = n - 1; a[l] <= a[k]; l--)
  ;

  /* 3. Swap a[k] with a[l] */
  swap(k, l);

  /* 4. Reverse the sequence from a[k + 1] to the end */
  for (k++, l = n - 1; l > k; l--, k++)
    swap(k, l);
  return 1;
#undef swap
}

void fisheryates_shuffle(int *array, int n, int passes) {

  int i, j, tmp, upper_bound;
  for(int p = 0; p < passes; p++) {
    for (i = n - 1; i > 0; i--) {

    upper_bound = RAND_MAX - ((RAND_MAX % (i + 1)) + 1);

    do {
      j = rand() % (i + 1);
    } while (j > upper_bound);

    tmp = array[j];
    array[j] = array[i];
    array[i] = tmp;
    }
  }

}
