#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "bruteforce.h"
#include "phenotype.h"
#include "reader.h"
#include "tsp.h"

#define THREADS 16
static graph_t graph;

static float best;
pthread_mutex_t best_lock;

void *run_bruteforce(void *p);

void start_bruteforce()
{
    pthread_t tid[THREADS];
    worker_t worker[THREADS];

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

    best = FLT_MAX;

    pthread_mutex_init(&best_lock, NULL);

    for(int w = 0; w < THREADS; w++){
      worker[w].wid = w;

      for(int i = 0; i < 24; i++){
        worker[w].genome_sequence[i] = i;
      }
      worker[w].genome.sequence = worker[w].genome_sequence;
      worker[w].genome.length = 23;


      shuffle(&worker[w].genome, 8);

      pthread_create(&tid[w], NULL, run_bruteforce, (void *) &worker[w]);
    }
    for(int w = 0; w < THREADS; w++){
      pthread_join(tid[w], NULL);
    }
}

void *run_bruteforce(void *p){
    worker_t *w = (worker_t *) p;
    node_t *n;
    nvertex_t *nlink;
    printf("Created thread %d\n", w->wid);
    float temp = 0;
    while(1){
      if((temp = evaluate(&w->genome, &graph)) < best){
        printf("wid: %d: New best %.2f\n", w->wid, temp);
        pthread_mutex_lock(&best_lock);
        n = graph.nodes[w->genome.sequence[0]];
        printf("[%s]", (char *) n->data);
        for(int i = 1; i < w->genome.length; i++){
            nlink = n->links[w->genome.sequence[i]];
            n = nlink->link;
            printf("->[%s]", (char *) n->data);
        }
        printf("\n\n");
        best = temp;
        pthread_mutex_unlock(&best_lock);
      }
      permute(w->genome.sequence, w->genome.length);
    }
}
