#include <stdio.h>
#include "tagstack.h"

int is_in_list(){
    return tag_in_stack(TAG_UL, 0)
        || tag_in_stack(TAG_INDENT_UL, 0);
}

int indent_level(char *s){
    int space = 0;
    while(*s){
        if('\t' == *s){
            space += 4;
        }
        if(' ' == *s){
            space++;
        }
        s++;
    }
    return space / 4;
}

int latest_list_level(){
    t_tag_item *item;

    item = tag_latest_stack(TAG_UL);
    if(item){
        return item -> indent_level; 
    }

    item = tag_latest_stack(TAG_INDENT_UL);
    if(item){
        return item -> indent_level; 
    }

    return -1;
}

char* tag_check_stack(t_tag tag, int level){
    if(TAG_BLANK != tag){
        tag_push_stack(tag, level);
    }
    return "";
}

void tag_show_stack(){
    t_tag_item *item;

    while( ( item = tag_shift_stack() ) ){
        switch(item -> tag){

            case TAG_P: printf("TAG_P, %d\n", item -> indent_level); break;
            case TAG_UL: printf("TAG_UL, %d\n", item -> indent_level); break;
            case TAG_OL: printf("TAG_OL, %d\n", item -> indent_level); break;
            case TAG_PRE: printf("TAG_PRE, %d\n", item -> indent_level); break;
            case TAG_QUOTE: printf("TAG_QUOTE, %d\n", item -> indent_level); break;
            case TAG_BLANK: printf("TAG_BLANK, %d\n", item -> indent_level); break;
            case TAG_H: printf("TAG_H, %d\n", item -> indent_level); break;
            case TAG_EOF: printf("TAG_EOF, %d\n", item -> indent_level); break;

            case TAG_QUOTE_P: printf("TAG_QUOTE_P, %d\n", item -> indent_level); break;
            case TAG_QUOTE_UL: printf("TAG_QUOTE_UL, %d\n", item -> indent_level); break;
            case TAG_QUOTE_OL: printf("TAG_QUOTE_OL, %d\n", item -> indent_level); break;
            case TAG_QUOTE_PRE: printf("TAG_QUOTE_PRE, %d\n", item -> indent_level); break;
            case TAG_QUOTE_BLANK: printf("TAG_QUOTE_BLANK, %d\n", item -> indent_level); break;
            case TAG_QUOTE_H: printf("TAG_QUOTE_H, %d\n", item -> indent_level); break;

            case TAG_INDENT_P: printf("TAG_INDENT_P, %d\n", item -> indent_level); break;
            case TAG_INDENT2_P: printf("TAG_INDENT2_P, %d\n", item -> indent_level); break;
            case TAG_INDENT3_P: printf("TAG_INDENT3_P, %d\n", item -> indent_level); break;
            case TAG_INDENT_UL: printf("TAG_INDENT_UL, %d\n", item -> indent_level); break;
            case TAG_INDENT_OL: printf("TAG_INDENT_OL, %d\n", item -> indent_level); break;
            case TAG_INDENT_PRE: printf("TAG_INDENT_PRE, %d\n", item -> indent_level); break;

            default: printf("ERROR TAG\n"); break;
        }
    }
}
