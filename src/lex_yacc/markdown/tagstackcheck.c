#include <stdio.h>
#include "tagstack.h"

void pop_bigger_level_tag(level){
    t_tag_item *item = tag_top_stack();

    while( item
        && item->indent_level >= level){
        tag_pop_stack();
        printf("pop tag: %s, level %d\n", get_tag_type(item->tag), item->indent_level);
        item = tag_top_stack();
    }
}

int is_in_list(int level){
    
    t_tag_item *item;

    item = tag_latest_stack(TAG_UL);
    if(item && item -> indent_level + 1 == level){
        return 1;
    }

    item = tag_latest_stack(TAG_INDENT_UL);
    if(item && item -> indent_level + 1 == level){
        return 1;
    }

    item = tag_latest_stack(TAG_OL);
    if(item && item -> indent_level + 1 == level){
        return 1;
    }

    item = tag_latest_stack(TAG_INDENT_OL);
    if(item && item -> indent_level + 1 == level){
        return 1;
    }

    return 0;

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

char* tag_check_stack(t_tag tag, int level){

    if(tag != TAG_BLANK && tag != TAG_QUOTE_BLANK){
        printf("current tag: %s, level: %d\n", get_tag_type(tag), level);
        pop_bigger_level_tag(level);
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
            case TAG_INDENT_UL: printf("TAG_INDENT_UL, %d\n", item -> indent_level); break;
            case TAG_INDENT_OL: printf("TAG_INDENT_OL, %d\n", item -> indent_level); break;
            case TAG_INDENT_PRE: printf("TAG_INDENT_PRE, %d\n", item -> indent_level); break;

            default: printf("ERROR TAG\n"); break;
        }
    }
}

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

