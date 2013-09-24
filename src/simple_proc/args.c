#include "args.h"

#include <stdio.h>

// exit, malloc, free
#include <stdlib.h>

// memset, strchr
#include <string.h>

char *items[MAX_ITEMS];

char **arg_split(char *args, char delimiter){
    char *start, *end, *tmp; 
    int size, count = 0, i;

    start = end = args;
    end = strchr(args, delimiter);
    memset(items, 0, sizeof(char *) * MAX_ITEMS);

    while(end != NULL
        && count < MAX_ITEMS){

        size = end - start + 1;
        // printf("pos: %d\n", end - args);

        tmp = (char *)malloc(size); 
        memset(tmp, 0, size);
        strncpy(tmp, start, size - 1);
        items[count++] = tmp;
        // printf("%s\n", tmp);

        start = end + 1;
        end = strchr(start, delimiter);

    }

    if(start != args){

        size = args + strlen(args) - start;
        tmp = (char *)malloc(size); 
        memset(tmp, 0, size);
        strncpy(tmp, start, size - 1);
        items[count++] = tmp;
        // printf("%s\n", tmp);

    }
    else{

        size = strlen(args);
        tmp = (char *)malloc(size); 
        memset(tmp, 0, size);
        strncpy(tmp, start, size - 1);
        items[count++] = tmp;
        // printf("%s\n", tmp);

    }

    // printf("%d items\n", count);

    // execvp的argv参数最后一项必须为NULL
    for(i=count; i<MAX_ITEMS; i++){
        items[i] = NULL;
    }

    // for(i=0; i<count; i++){
    //     printf("\"%s\" ", items[i]);
    // }
    // printf("\n");

    return items;
}

void arg_free(){
	int i;
	for(i=0; i<MAX_ITEMS; i++){
		if(items[i] != NULL){
			free(items[i]);
		}
	}
}

