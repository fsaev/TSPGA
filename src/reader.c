#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
static FILE *fp;

int open_csv(char *path) {
    fp = fopen(path, "r");
    return (fp == NULL);
}

char** line_csv(int *entries_read) {

    char **entries = 0;
    int ecnt = 0;

    char buf[1024];
    char *tok = 0;

    if(!fp){ return entries; }

    if(fgets(buf, 1024, fp)){ //Get
        tok = strtok(buf, ";");

        while(tok != NULL){ //Get each value
            entries = realloc(entries, sizeof(char *) * (ecnt + 1));
            char *loc = 0;
            if((loc = strchr(tok, '\n'))){ //Look for any newline chars
                *loc = '\0'; //Move termination to location
            }
            if((loc = strchr(tok, '\r'))){ //Look for any CR chars
                *loc = '\0'; //Move termination to location
            }
            entries[ecnt] = malloc(sizeof(char) * strlen(tok));
            strcpy(entries[ecnt], tok);
            tok = strtok(NULL, ";");
            ecnt++;
        }
    }

    if(entries){
        entries[ecnt] = NULL; //NULL last entry
    }

    *entries_read = ecnt;
    return entries;
}