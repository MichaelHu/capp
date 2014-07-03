#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "htmltags.h"
#include "blocknode.h"

char *get_tag_type(t_tag tag);

char *get_tag_type(t_tag tag){
    switch(tag){
        case TAG_NULL: return "TAG_NULL";

        case TAG_P: return "TAG_P";
        case TAG_UL: return "TAG_UL";
        case TAG_OL: return "TAG_OL";
        case TAG_PRE: return "TAG_PRE";
        case TAG_QUOTE: return "TAG_QUOTE";
        case TAG_BLANK: return "TAG_BLANK";
        case TAG_H: return "TAG_H";
        case TAG_EOF: return "TAG_EOF";

        case TAG_QUOTE_P: return "TAG_QUOTE_P";
        case TAG_QUOTE_UL: return "TAG_QUOTE_UL";
        case TAG_QUOTE_OL: return "TAG_QUOTE_OL";
        case TAG_QUOTE_PRE: return "TAG_QUOTE_PRE";
        case TAG_QUOTE_BLANK: return "TAG_QUOTE_BLANK";
        case TAG_QUOTE_H: return "TAG_QUOTE_H";

        case TAG_INDENT_P: return "TAG_INDENT_P";
        case TAG_INDENT_UL: return "TAG_INDENT_UL";
        case TAG_INDENT_OL: return "TAG_INDENT_OL";
        case TAG_INDENT_PRE: return "TAG_INDENT_PRE";
    }
}

t_blocknode *blocknode_create(t_tag tag, int nops, ...){
    va_list args;
    t_blocknode *p;
    int i;

    if( ( p = 
            (t_blocknode *)malloc(sizeof(t_blocknode)) ) 
        == NULL){
        printf("out of memory");
    }

    if( ( p -> ops = 
            (char **)malloc(nops * sizeof(char *)) ) 
        == NULL){
        printf("out of memory");
    }

    p->tag = tag; 
    p->nops = nops;
    va_start(args, nops);
    for(i=0; i<nops; i++){
        p->ops[i] = va_arg(args, char*);
    }
    va_end(args);
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

