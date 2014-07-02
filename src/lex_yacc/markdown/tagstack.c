#include <stddef.h> /* for NULL */
#include <stdlib.h>
#include "tagstack.h"

t_tag_stack tag_stack;

void tag_init_stack(){
    int i = 0;
    tag_stack.size = 0;
    for(; i<TAG_STACK_SIZE; i++){
        tag_stack.arr[i] = NULL;
    }
}

int tag_empty_stack(){
    return tag_stack.size == 0;
}

int tag_full_stack(){
    return tag_stack.size == TAG_STACK_SIZE;
}

t_tag_item *tag_top_stack(){
    if(tag_empty_stack()){
        return NULL;
    }
    else{
        return tag_stack.arr[tag_stack.size - 1];
    }
}

void tag_push_stack(t_tag tag, int level){
    t_tag_item *item;

    if(tag_full_stack()){
        return;
    }

    item = (t_tag_item *)malloc(sizeof(t_tag_item));
    if(item){
        item -> tag = tag;
        item -> indent_level = level;
    }

    tag_stack.arr[tag_stack.size++] = item;
}

t_tag_item *tag_pop_stack(){
    if(tag_empty_stack()){
        return NULL;
    }
    return tag_stack.arr[--tag_stack.size];
}

t_tag_item *tag_shift_stack(){
    t_tag_item *item;
    int i;

    if(tag_empty_stack()){
        return NULL;
    }
    item = tag_stack.arr[0];

    for(i=0; i<tag_stack.size - 1; i++){
        tag_stack.arr[i] = tag_stack.arr[i+1];
    }
    tag_stack.size --;

    return item;
}

int tag_in_stack(t_tag tag, int level){
    int i;
    for(i=0; i<tag_stack.size; i++){
        if(tag == tag_stack.arr[i] -> tag
            && level == tag_stack.arr[i] -> indent_level){
            return 1;
        }
    }
    return 0;
}

t_tag_item *tag_latest_stack(t_tag tag){
    int i;
    for(i=0; i<tag_stack.size; i++){
        if(tag == tag_stack.arr[i] -> tag){
            return tag_stack.arr[i];
        }
    }
    return NULL;
}
