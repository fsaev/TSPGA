#include <stdlib.h>
#include "bruteforce.h"
#include "tsp.h"
#include "permute.h"

void start_bruteforce()
{
    int node_count = get_node_count();
    int* genome = malloc(sizeof(int) * node_count);
    for(int i = 0; i < node_count; i++){
        genome[i] = i;
    }
    permute(genome, node_count, measure_fitness);
}