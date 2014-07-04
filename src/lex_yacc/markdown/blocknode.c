#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "htmltags.h"
#include "blocknode.h"

t_blocknode *node_list = NULL, *node_current = NULL;

/* inner function prototypes */
void pop_higher_level_blocknode(int level);
char *blocknode_glue(t_blocknode *top, t_blocknode *current);
char *blocknode_output(t_blocknode *node);







void pop_higher_level_blocknode(level){
    t_blocknode *item = blocknode_top_stack();

    while( item
        && item->indent_level >= level){
        blocknode_pop_stack();
        /*
        printf("pop tag: %s, level %d\n", get_tag_type(item->tag), item->indent_level);
        */
        item = blocknode_top_stack();
    }
}

char *blocknode_glue(t_blocknode *top, t_blocknode *current){
    return "";
}

char *blocknode_output(t_blocknode *node){
    switch(node->tag){
        case TAG_NULL: printf(""); break;

        case TAG_P: printf("%s", node->ops[0]); break;
        case TAG_OL:
        case TAG_UL: printf("<li>%s", node->ops[0]); break;
        case TAG_PRE: printf("%s", node->ops[1]); break;

        case TAG_QUOTE: printf(""); break;
        case TAG_BLANK: printf(""); break;

        case TAG_H: printf(""); break;
        case TAG_EOF: printf(""); break;

        case TAG_QUOTE_P: printf(""); break;
        case TAG_QUOTE_UL: printf(""); break;
        case TAG_QUOTE_OL: printf(""); break;
        case TAG_QUOTE_PRE: printf(""); break;
        case TAG_QUOTE_BLANK: printf(""); break;
        case TAG_QUOTE_H: printf(""); break;

        case TAG_INDENT_P: printf(""); break;
        case TAG_INDENT_UL: printf(""); break;
        case TAG_INDENT_OL: printf(""); break;
        case TAG_INDENT_PRE: printf(""); break;
    }

    return "";
}





t_blocknode *blocknode_create(t_tag tag, int level, int nops, ...){
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
    p->indent_level = level; 
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

char *blocknode_show(t_blocknode *node){
    char *s, *format;

    switch(node -> nops){
        case 1:
            format = "%-*s (%d):\t\t[%s]";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
                , node->ops[0]
            );
            break;
        case 2:
            format = "%-*s (%d):\t\t[%s], [%s]";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
                , node->ops[0]
                , node->ops[1]
            );
            break;
        case 3:
            format = "%-*s (%d):\t\t[%s], [%s], [%s]";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
                , node->ops[0]
                , node->ops[1]
                , node->ops[2]
            );
            break;

        default:
            format = "%-*s (%d)";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
            );
            break;
    }

    printf("blocknode: %s\n", s);

    return s;
}

char* blocknode_parse(t_blocknode *node){
    return "";
}

void blocklist_parse(){
    t_blocknode *p = node_list;

    while(p){

        pop_higher_level_blocknode(p->indent_level);
        blocknode_push_stack(p);
        blocknode_output(p);

        /*
        blocknode_show(p);
        */
        p = p->next;
    }

}
