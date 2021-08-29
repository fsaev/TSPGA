#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "reader.h"
#include "tsp.h"
#include "interface.h"

#include "reader_tb.h"
#include "phenotype.h"

int main(int argc, char *argv[])
{
    char *g_entries[30];
    graph_t tsp_graph;


    //if(argc < 2){ return 1; }
    if(!open_csv(argv[1])){
    //if(!open_csv("../assets/european_cities.csv")){
        printf("File \"%s\" opened successfully\n\r", argv[1]);
    }else{
        printf("File \"%s\" could not be opened\n\r", argv[1]);
    }

    /*
     * Read Header
     */
    int len = line_csv(g_entries, 30);
    if(len){
        create_graph_nodes(&tsp_graph, g_entries, len);
    }

    /*
     * Read Data
     */
    int i = 0;
    while((len = line_csv(g_entries, 30))){
        if(len){
            link_graph_node(&tsp_graph, i++, g_entries, len);
        }
    }

    //start_ga(tsp_graph, 10, 8);

    return 0;
}