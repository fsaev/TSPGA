

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

  return 0;
}

void *run_ga(void *p) {

}
