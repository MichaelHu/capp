#include <stdio.h>
#include "tagstack.h"

void pop_bigger_level_tag(level){
    t_tag_item *item = tag_top_stack();

    while( item
        && item->indent_level >= level){
        tag_pop_stack();
#ifdef MD_DEBUG
        printf("pop tag: %s, level %d\n", get_tag_type(item->tag), item->indent_level);
#endif
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

int inner_pre_level(int level){
    
    t_tag_item *item;
    int _level = -1;

    item = tag_latest_stack(TAG_UL);
    if(item && level >= item -> indent_level + 2){
        if(_level < item -> indent_level + 1){
            _level = item -> indent_level + 1;
        }
    }

    item = tag_latest_stack(TAG_INDENT_UL);
    if(item && level >= item -> indent_level + 2){
        if(_level < item -> indent_level + 1){
            _level = item -> indent_level + 1;
        }
    }

    item = tag_latest_stack(TAG_OL);
    if(item && level >= item -> indent_level + 2){
        if(_level < item -> indent_level + 1){
            _level = item -> indent_level + 1;
        }
    }

    item = tag_latest_stack(TAG_INDENT_OL);
    if(item && level >= item -> indent_level + 2){
        if(_level < item -> indent_level + 1){
            _level = item -> indent_level + 1;
        }
    }

    return _level;

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
#ifdef MD_DEBUG
        printf("current tag: %s, level: %d\n", get_tag_type(tag), level);
#endif
        pop_bigger_level_tag(level);
        tag_push_stack(tag, level);
    }
    return "";

}

char *get_tag_type(t_tag tag){
    switch(tag){
        case TAG_ERROR: return "TAG_ERROR";

        case TAG_P: return "TAG_P";
        case TAG_UL: return "TAG_UL";
        case TAG_OL: return "TAG_OL";
        case TAG_PRE: return "TAG_PRE";
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

