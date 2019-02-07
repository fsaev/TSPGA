#include <stdio.h>
#include "interface.h"
#include "bruteforce.h"

void run_interface() {
    int print_menu = 1;
    char selection = 0;

    if(print_menu){
        printf("==== MENU ====\n\r");
        printf("1: Run Bruteforce\n\r");
        printf("2: Run Genetic Algorithm\n\r");
        print_menu = 0;
    }

    if(scanf(" %1c", &selection)){
        printf("found %1c\n\r", selection);
        print_menu = 1;
        switch(selection){
            case '1':
                printf("Bruteforcing\n\r");
                start_bruteforce();
                break;
            case '2':
                //apply_ga();
                break;
            default:
                printf("Invalid argument\n\r");
        }
    }
    
}