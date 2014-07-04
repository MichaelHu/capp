#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "htmltags.h"
#include "blocknode.h"

t_blocknode *node_list = NULL, *node_current = NULL;

t_blocknode *blocknode_create(t_tag tag, int nops, ...){
    va_list args;
    t_blocknode *p;
    int i;

    if( ( p = 
            (t_blocknode *)malloc(sizeof(t_blocknode)) ) 
        == NULL){
        printf("out of memory");
        exit(1);
    }

    if( ( p -> ops = 
            (char **)malloc(nops * sizeof(char *)) ) 
        == NULL){
        printf("out of memory");
        exit(1);
    }

    p->tag = tag; 
    p->nops = nops;
    p->next = NULL;
    va_start(args, nops);
    for(i=0; i<nops; i++){
        p->ops[i] = va_arg(args, char*);
    }
    va_end(args);


    if(!node_list){
        node_list = p;
    }
    if(node_current){
        node_current -> next = p;
    }
    node_current = p;

    return p;
}

char *blocknode_parse(t_blocknode *node){
    char *s, *format;

    switch(node -> nops){
        case 1:
            format = "%s, [ %s ]";
            s = str_format(
                format
                , get_tag_type(node->tag)
                , node->ops[0]
            );
            break;
        case 2:
            format = "%s, [ %s ], [ %s ]";
            s = str_format(
                format
                , get_tag_type(node->tag)
                , node->ops[0]
                , node->ops[1]
            );
            break;
        case 3:
            format = "%s, [ %s ], [ %s ], [ %s ]";
            s = str_format(
                format
                , get_tag_type(node->tag)
                , node->ops[0]
                , node->ops[1]
                , node->ops[2]
            );
            break;

        default:
            format = "%s";
            s = str_format(
                format
                , get_tag_type(node->tag)
            );
            break;
    }

    printf("blocknode: %s\n", s);

    return s;
}

void blocklist_parse(){
    t_blocknode *p = node_list;

    while(p){
        blocknode_parse(p);
        p = p->next;
    }

}
