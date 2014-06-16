#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "htmltags.h" 

char *str_concat(char *s1, char *s2){
    char *_str;

    _str = (char *)malloc(strlen(s1) + strlen(s2) + 1);     
    sprintf(_str, "%s%s", s1, s2); 
    return _str;
}

char *str_format(char *format, ...){
    char *_str = NULL;
    va_list args; 

    _str = (char *)malloc(1000000);     
    va_start(args, format);
    vsprintf(_str, format, args); 
    va_end(args);
    return _str;
}

char *create_image(char *title, char *href){
    char *_str = NULL,
        *format = "<img src=\"%s\" title=\"%s\">",
        *pos = NULL;

    pos = strchr(href, ' ');

    if(pos != NULL){
        *pos = '\0';
    }

    _str = (char *)malloc(
        strlen(title) 
        + strlen(format) - 4 
        + strlen(href)
        + 1
    );     
    sprintf(_str, format, href, title); 
    return _str;
}

char *create_link(char *title, char *href){
    char *_str = NULL,
        *format = "<a href=\"%s\">%s</a>",
        *pos = NULL;

    pos = strchr(href, ' ');

    if(pos != NULL){
        *pos = '\0';
    }

    _str = (char *)malloc(
        strlen(title) 
        + strlen(format) - 4 
        + strlen(href)
        + 1
    );     
    sprintf(_str, format, href, title); 
    return _str;
}

char *create_hn(char *s, int level){
    char *_str,
        *format = "<h%d>%s</h%d>\n";

    _str = (char *)malloc(
        strlen(s) 
        + strlen(format) - 2 
        + 1
    );     
    sprintf(_str, format, level, s, level); 
    return _str;
}

char *create_emphasis(char *s){
    char *_str,
        *format = "<em>%s</em>";

    _str = (char *)malloc(
        strlen(s) 
        + strlen(format) - 2 
        + 1
    );     
    sprintf(_str, format, s); 
    return _str;
}

char *create_strong(char *s){
    char *_str,
        *format = "<strong>%s</strong>";

    _str = (char *)malloc(
        strlen(s) 
        + strlen(format) - 2 
        + 1
    );     
    sprintf(_str, format, s); 
    return _str;
}


