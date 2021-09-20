#define _DEFAULT_SOURCE

#include <time.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "ga.h"
#include "reader.h"
#include "tsp.h"

#define PMX_MINIMUM_SUBSTRING 3
#define SHUFFLE_ITERATIONS 8
#define SELECTION_SIZE 10 //TODO assert this value
#define MUTATION_FRACTION 8

typedef struct{
  int wid;
  pthread_t thread;

  /* Must take care of concurrency */
  int best_fitness_generation;
  individual_t *best_fitness_individual;
  pthread_mutex_t best_fitness_lock;

  int generation;

  individual_t *individuals;
  int individuals_size;
} worker_t;

/* Shared data */
static graph_t graph;

/* Static function prototypes */
void enter_master_loop(worker_t *worker, const int worker_count);
void *run_ga_slave(void *p);

/* Reverses a random fraction of the genome */
static void invertion_mutate(individual_t *o, const int mutation_fraction) {
  int substring_s, substring_e;
  int tmp;

  substring_s = rand() % (o->genome.length - (o->genome.length/mutation_fraction));
  substring_e = substring_s + (o->genome.length/mutation_fraction);

  for(int i = substring_s; i < substring_e; i++){
    tmp = o->genome.sequence[i];
    o->genome.sequence[i] = o->genome.sequence[substring_e - i - 1];
    o->genome.sequence[substring_e - i - 1] = tmp;
    if(i >= (substring_e - i - 1)){ //Quit half way
      return;
    }
  }
}

/* Recursive helper function for the PMX crossover */
static int recursive_positionize(const genome_t *p1, const genome_t *p2, genome_t *o1, const int idx){
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

/*
 * Merges two individuals into one by taking a section from each
 * then repositioning the genomes that are duplicated
 */
void pmx_crossover(const genome_t *p1, const genome_t *p2, genome_t *o1) {
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
      int pos = recursive_positionize(p1, p2, o1, p2_idx);
      if(pos != -1){
        o1->sequence[pos] = p2->sequence[p2_idx];
      }
    }
  }

  /* Copy remaining free slots from P2 to O1*/
  for(int i = 0; i < o1->length; i++){
    if(o1->sequence[i] == -1){
      o1->sequence[i] = p2->sequence[i];
    }
  }
}

/*
 * Evaluation function
 * Iterates each individual, traverses the chain
 * then assigns a fitness based on the total length
 */
static float ga_evaluate(individual_t *individuals, const int size){
  float fitness_sum = 0;

  /* Evaluate */
  for(int i = 0; i < size; i++){
    individual_t *n = &individuals[i];
    n->fitness = tsp_evaluate(&n->genome, &graph);
    fitness_sum += n->fitness;
  }

  return fitness_sum;
}

/* Qsort comparison function */
static int cmpfunc (const void *a, const void *b){
  const individual_t *ia = (individual_t *) a;
  const individual_t *ib = (individual_t *) b;
  return (int) ( ia->fitness - ib->fitness );
}

/*
 * Selection function
 * Fitness proportionate selection with higher fitness meaning
 * higher probability of being selected
 */
static void ga_select(individual_t *individuals, const int individuals_size, individual_t **winners, const int winners_size, const float fitness_sum){
  float prev_probability = 0;
  double winning_no[winners_size];

  qsort(individuals, individuals_size, sizeof(individual_t), cmpfunc);

  /* Prepare winning numbers */
  for(int i = 0; i < winners_size; i++){
    winning_no[i] = (double)rand() / (double)RAND_MAX;

    /*Set initial winner to best individual */
    winners[i] = &individuals[0];
  }

  /* Do the raffle */
  for(int i = 0; i < individuals_size - winners_size; i++){ //For every individual except the weakest
    individual_t *n = &individuals[i];
    n->s_prob = prev_probability + (n->fitness/fitness_sum);

    /* Also evaluate if individual is selected, yay NP-completeness */
    for(int s = 0; s < winners_size; s++){
      if(winning_no[s] > n->s_prob){
        winners[s] = n;
      }
    }

    prev_probability = n->s_prob;
  }
}

/*
 * Reproduction function
 * Wipes the weakest individuals
 * with the offspring of the selected individuals
 *
 * Select parents on each of the extremes of the selection group
 * Replace the individuals from the bottom and up
 *
 * Uses PMX crossover to create the offspring, then applies
 * inversion mutation to introduce some randomness into the
 * gene pool
 */
static void ga_reproduce(individual_t *individuals, const int individuals_size, individual_t **winners, const int winners_size)
{
  for(int s = 0; s < winners_size; s++){
    pmx_crossover(&winners[s]->genome, &winners[winners_size - s - 1]->genome, &individuals[individuals_size - s - 1].genome);
    invertion_mutate(&individuals[individuals_size - s - 1], MUTATION_FRACTION);
  }
}

/*
 * Entry function
 * Prepares data
 */
int start_ga(const int threads, const int start_population) {
  worker_t worker[threads];

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

  printf("Spawning %d threads with %d individuals each\n",
          threads, start_population/threads);

  /* Set up workers */
  for(int i = 0; i < threads; i++){
    worker_t* w = &worker[i];
    w->wid = i;
    w->generation = 0;
    w->individuals_size = start_population/threads; //Only allocate an even share
    printf("Allocating %d individuals\n", w->individuals_size);
    w->individuals = malloc(sizeof(individual_t) * w->individuals_size);

    for(int g = 0; g < w->individuals_size; g++){
      genome_t *genome = &w->individuals[g].genome;
      genome->sequence = malloc(sizeof(int) * 23);
      genome->length = 23;
      for(int n = 0; n <= 23; n++){
        genome->sequence[n] = n;
      }
      fisheryates_shuffle(genome->sequence, genome->length, SHUFFLE_ITERATIONS);
    }

    w->best_fitness_individual = &w->individuals[0];
    pthread_mutex_init(&w->best_fitness_lock, NULL);
    pthread_create(&worker[i].thread, NULL, run_ga_slave, (void *) &worker[i]);
  }

  enter_master_loop(worker, threads);

  return 0;
}

/*
 * Master loop for the main thread
 * Gathers the best result from each worker
 * every 100 milliseconds
 */
void enter_master_loop(worker_t *worker, const int worker_count){
  float best_fitness = FLT_MAX;
  individual_t *best_individual;

  while(1){
    for(int i = 0; i < worker_count; i++){
      if(worker[i].generation == 0){ continue; } //Skip first generation

      pthread_mutex_lock(&worker[i].best_fitness_lock);
      best_individual = worker[i].best_fitness_individual;

      if(best_individual->fitness < best_fitness){
        best_fitness = best_individual->fitness;
        printf("Best fitness: %f (wid: %d, g: %d)\n",
            best_fitness, i, worker[i].best_fitness_generation);
        for(int n = 0; n < best_individual->genome.length; n++){
          int seq_no = best_individual->genome.sequence[n];
          if(n != (best_individual->genome.length - 1)){
            printf("[%s]->", (char *) graph.nodes[seq_no]->data);
          }else{
            printf("[%s]\n\n", (char *) graph.nodes[seq_no]->data);
          }
        }
      }
      pthread_mutex_unlock(&worker[i].best_fitness_lock);

    }
    usleep(100); //Slow down master thread
  }

  /*
   * The threads will probably never end
   * (We are working on the traveling salesman problem here, not the halting problem)
   * But we keep the join functions just to be nice and orderly, and also to keep
   * the main thread from exiting.
   */
  for(int i = 0; i < worker_count; i++){
    pthread_join(worker[i].thread, NULL);
  }
}

/*
 * Worker thread
 * pthread instance that keeps it's own
 * set of data, but reads the same graph
 */
void *run_ga_slave(void *p) {
  worker_t *w = (worker_t *) p;
  float fitness_sum = 0;
  individual_t *winners[SELECTION_SIZE];

  printf("Starting worker thread %d\n", w->wid);
  while(1){
    w->generation++; //increment generation
    fitness_sum = ga_evaluate(w->individuals, w->individuals_size);
    ga_select(w->individuals, w->individuals_size, winners, SELECTION_SIZE, fitness_sum);

    pthread_mutex_lock(&w->best_fitness_lock);
    w->best_fitness_individual = &w->individuals[0];
    w->best_fitness_generation = w->generation;
    pthread_mutex_unlock(&w->best_fitness_lock);

    ga_reproduce(w->individuals, w->individuals_size, winners, SELECTION_SIZE);
  }
  return 0;
}
