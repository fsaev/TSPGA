#include <stdlib.h>
#include <time.h>
#include "ga.h"

int main(int argc, char *argv[])
{

    srand(time(NULL));
    start_ga(16, 1000);

    return 0;
}
