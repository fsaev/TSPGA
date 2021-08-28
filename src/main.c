#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "reader.h"
#include "tsp.h"
#include "interface.h"

char *g_entries[30];

int main(int argc, char *argv[])
{
    printf("Hello\n\r");
    if(argc < 2){ return 1; }
    if(!open_csv(argv[1])){
        printf("File \"%s\" opened successfully\n\r", argv[1]);
    }else{
        printf("File \"%s\" could not be opened\n\r", argv[1]);
    }

    /*
     * Read Header
     */
    int len = line_csv(g_entries, 30);
    if(len){
        create_graph_nodes(g_entries, len);
    }

    /*
     * Read Data
     */
    int i = 0;
    while((len = line_csv(g_entries, 30))){
        if(len){
            link_graph_node(i++, g_entries, len);
        }
    }

    while(1){
        run_interface();
    }
    
    return 0;
}