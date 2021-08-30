#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "reader.h"
#include "tsp.h"
#include "interface.h"

#include "reader_tb.h"
#include "phenotype.h"
#include "bruteforce.h"

int main(int argc, char *argv[])
{

    srand(time(NULL));
    start_bruteforce();

    return 0;
}
