#include "tagstack.h"

t_tag_stack tag_stack;

char* tag_check_stack_ul();
char* tag_check_stack_ol();
char* tag_check_stack_p();
char* tag_check_stack_pre();
char* tag_check_stack_other();

void tag_init_stack(){
    int i = 0;
    tag_stack.size = 0;
    for(; i<10; i++){
        tag_stack.arr[i] = TAG_OTHER;
    }
}

int tag_empty_stack(){
    return tag_stack.size == 0;
}

int tag_full_stack(){
    return tag_stack.size == 10;
}

t_tag tag_top_stack(){
    if(tag_empty_stack()){
        return TAG_NULL;
    }
    else{
        return tag_stack.arr[tag_stack.size - 1];
    }
}

void tag_push_stack(t_tag tag){
    if(tag_full_stack()){
        return;
    }
    tag_stack.arr[tag_stack.size++] = tag;
}

t_tag tag_pop_stack(){
    if(tag_empty_stack()){
        return TAG_NULL;
    }
    return tag_stack.arr[tag_stack.size--];
}

char* tag_check_stack(t_tag tag){
    switch(tag){
        case TAG_P:
            return tag_check_stack_p();
        case TAG_UL:
            return tag_check_stack_ul();
        case TAG_OL:
            return tag_check_stack_ol();
        case TAG_PRE:
            return tag_check_stack_pre();
        default:
            return tag_check_stack_other();
    }
}

char* tag_check_stack_pre(){
    char *ret;
    switch(tag_top_stack()){
        case TAG_PRE:
            ret = "\n";
            break;

        case TAG_P:
            tag_pop_stack();
            ret = "</p>\n<pre><code>";
            break;

        case TAG_UL:
            tag_pop_stack();
            ret = "</ul>\n<pre><code>";
            break;

        case TAG_OL:
            tag_pop_stack();
            ret = "</ol>\n<pre><code>";
            tag_push_stack(TAG_P);
            break;

        default:
            tag_push_stack(TAG_PRE);
            ret = "\n<pre><code>";
    }
    return ret;
}

char* tag_check_stack_p(){
    char *ret;
    switch(tag_top_stack()){
        case TAG_P:
            ret = "";
            break;

        case TAG_UL:
            tag_pop_stack();
            ret = "</ul>\n<p>";
            break;

        case TAG_OL:
            tag_pop_stack();
            ret = "</ol>\n<p>";
            tag_push_stack(TAG_P);
            break;

        case TAG_PRE:
            tag_pop_stack();
            ret = "\n</code></pre>\n<p>";
            break;

        default:
            tag_push_stack(TAG_P);
            ret = "\n<p>";
    }
    return ret;
}

char* tag_check_stack_ul(){
    char *ret;

    switch(tag_top_stack()){
        case TAG_UL:
            ret = "";
            break;

        case TAG_P:
            tag_pop_stack();
            ret = "</p>\n<ul>";
            tag_push_stack(TAG_UL);
            break;

        case TAG_OL:
            tag_pop_stack();
            ret = "</ol>\n<ul>";
            tag_push_stack(TAG_UL);
            break;

        case TAG_PRE:
            tag_pop_stack();
            ret = "\n</code></pre>\n<ul>";
            break;

        default:
            tag_push_stack(TAG_UL);
            ret = "\n<ul>";
    }

    return ret;
}

char* tag_check_stack_ol(){
    char *ret;

    switch(tag_top_stack()){
        case TAG_OL:
            ret = "";
            break;

        case TAG_P:
            tag_pop_stack();
            ret = "</p>\n<ol>";
            tag_push_stack(TAG_OL);
            break;

        case TAG_UL:
            tag_pop_stack();
            ret = "</ul>\n<ol>";
            tag_push_stack(TAG_OL);
            break;

        case TAG_PRE:
            tag_pop_stack();
            ret = "\n</code></pre>\n<ol>";
            break;

        default:
            tag_push_stack(TAG_OL);
            ret = "\n<ol>";
    }

    return ret;
}

char* tag_check_stack_other(){
    char *ret;

    switch(tag_top_stack()){
        case TAG_P:
            tag_pop_stack();
            ret = "</p>\n";
            break;

        case TAG_UL:
            tag_pop_stack();
            ret = "</ul>\n";
            break;
            
        case TAG_OL:
            tag_pop_stack();
            ret = "</ol>\n";
            break;

        case TAG_PRE:
            tag_pop_stack();
            ret = "\n</code></pre>\n";
            break;

        default:
            tag_pop_stack();
            ret = "";
    }

    return ret;
}

