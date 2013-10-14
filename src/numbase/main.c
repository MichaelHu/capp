#include <stdio.h>
#include <stdlib.h>

void process(char *, int);

int main(int argc, char ** argv){
    int base = 10;
    char *num_str = NULL;

    if(2 == argc){
        num_str = argv[1];
    }

    if(3 == argc){
        num_str = argv[1];
        base = strtod(argv[2], NULL);
    }

    // printf("%s, %d\n", num_str, base); 
    process(num_str, base);
}

void process(char *num_str, int base){
    printf("%ld\n", strtol(num_str, NULL, base)); 
}

