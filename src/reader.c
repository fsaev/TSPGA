#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
static FILE *fp;

int open_csv(char *path) {
    fp = fopen(path, "r");
    return (fp == NULL);
}

int line_csv(char **entries, int len) {
    if(!fp){ return -1; }

    char buf[1024];
    char *tok = 0;
    int ecnt = 0;

    if(fgets(buf, 1024, fp)){ //Get
        tok = strtok(buf, ";");

        while(len-- && tok != NULL){ //Get each value
            char *loc = 0;
            if((loc = strchr(tok, '\n'))){ //Look for any newline chars
                *loc = '\0'; //Move termination to location
            }
            if((loc = strchr(tok, '\r'))){ //Look for any CR chars
                *loc = '\0'; //Move termination to location
            }
            ecnt++;
            *entries = malloc(sizeof(char) * strlen(tok));
            strcpy(*entries++, tok);
            tok = strtok(NULL, ";");
        }
    }

    *entries = NULL; //NULL last entry
    return ecnt;
}