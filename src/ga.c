

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ga.h"
#include "reader.h"

/*
 * Genetric algorithms overview
 * Create a population of N individuals
 * Individuals has a genome describing sequence
 * Each new individual shall be unique
 * Make unique by shuffling with Fisher-Yates
 *
 * Iteration:
 * Evaluate each individuals' fitness
 * Do fitness proportionate selection
 *  Basically a roulette wheel with stronger
 *  individuals having more chances
 *  This will make sure that weak individuals
 *  have a chance of reproduction as well
 * With the surviving individuals, do PMX Crossover

*/

/* Shared data */
static graph_t graph;
static genome_t *individuals;

void *run_ga(void *p);

  #define PMX_MINIMUM_SUBSTRING 3

static int recursive_positionize(genome_t *p1, genome_t *p2, genome_t *o1, int idx){
  for(int i = 0; i <= p2->length; i++){
    if(p1->sequence[idx] == p2->sequence[i]){
      if(o1->sequence[i] == -1){
        return i;
      }else{
        i = recursive_positionize(p1, p2, o1, i);
        return i;
      }
    }
  }

  printf("Corrupted genomes\n");
  return -1;
}

void pmx_crossover(genome_t *p1, genome_t *p2, genome_t *o1) {
  int substring_s, substring_e;

  /* Random end between 3 and n */
  substring_e = (rand() % (p1->length - PMX_MINIMUM_SUBSTRING)) + PMX_MINIMUM_SUBSTRING;
  /* Random start with a minimum backwards offset of minimum 3 and a maximum of 3 + (n/4) */
  substring_s = substring_e - ((rand() % p1->length/4) + PMX_MINIMUM_SUBSTRING);

  if(substring_s < 0){ substring_s = 0; } //Saturate to positive values

  memset(o1->sequence, -1, sizeof(int) * o1->length);

  /* Cross copy substrings to offsprings */
  memcpy(&o1->sequence[substring_s], &p1->sequence[substring_s], sizeof(int) * (substring_e - substring_s));

  int match_at;
  for(int p2_idx = substring_s; p2_idx < substring_e; p2_idx++){ //Parent iterator
    match_at = -1;
    for(int p1_idx = substring_s; p1_idx < substring_e; p1_idx++){ //Offspring iterator
      if(p1->sequence[p1_idx] == p2->sequence[p2_idx]){
        match_at = p2_idx;
        break;
      }
    }

    if(match_at == -1){ //If it's unique and needs repositioning
        o1->sequence[recursive_positionize(p1, p2, o1, p2_idx)] = p2->sequence[p2_idx];
    }
  }

  /* Copy remaining free slots from P2 to O1*/
  for(int i = 0; i < o1->length; i++){
    if(o1->sequence[i] == -1){
      o1->sequence[i] = p2->sequence[i];
    }
  }
}

static void evaluate(genome_t *g)
{
  // for (int i = 0; i < individuals->; i++)
  // {
  //   /* code */
  // }

}

static void select(genome_t *g)
{

}

static void reproduce(genome_t *g)
{

}

int ga_process_cycle(void *argp)
{

}

int start_ga(int threads, int start_population) {
  pthread_t tid[threads];
  //worker_t worker[threads];

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

  individuals = malloc(sizeof(genome_t) * start_population);

  for(int i = 0; i < start_population; i++){
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

  return 0;
}

void *run_ga(void *p) {

}
