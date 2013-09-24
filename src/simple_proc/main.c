#include <stdio.h>

// exit, malloc, free
#include <stdlib.h>

// memset, strchr
#include <string.h>

// fork
#include <unistd.h>

#include <errno.h>
#include <sys/types.h>

// wait
#include <sys/wait.h>

#include "args.h"


char cmd[256];

int main(int argc, char **argv){
    int rtn;
    char **items;
    
    while(1){
        printf("> ");
        // 包含末尾的换行符
        fgets(cmd, 256, stdin);
        if(0 == fork()){
            // execlp("ls", "ls", "-al", "/etc/passwd", (char *)0);

            items = arg_split(cmd, ' ');
            execvp(items[0], items);

            perror("failed ");
            exit(errno);
        }
        else{
            wait(&rtn);
            // printf("child process return %d\n", rtn);
            printf("exit status: %d\n", rtn);
        }
    }

}

