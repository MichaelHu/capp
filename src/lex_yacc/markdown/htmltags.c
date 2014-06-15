#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htmltags.h" 

char *create_link(char *title, char *href){
    char *_str,
        *format = "<a href=\"%s\">%s</a>";

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
        *format = "<h%d>%s</h%d>";

    _str = (char *)malloc(
        strlen(s) 
        + strlen(format) - 2 
        + 1
    );     
    sprintf(_str, format, level, s, level); 
    return _str;
}

char *str_concat(char *s1, char *s2){
    char *_str;

    _str = (char *)malloc(strlen(s1) + strlen(s2) + 1);     
    sprintf(_str, "%s%s", s1, s2); 
    return _str;
}



