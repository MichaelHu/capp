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

char *str_padding_left(char *s, int count){
    char *_str, *ret;

    if(0 == count){
        return s;
    }

    _str = (char *)malloc(count + 1);     
    if(!_str){
        printf("out of memory!\n");
        exit(1);
    }

    memset(_str, ' ', count);
    _str[count] = 0;

    ret = str_concat(_str, s);
    free(_str);

    return ret;
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

char *create_codespan(char *s){
    char *_str,
        *format = "<code>%s</code>";

    _str = (char *)malloc(
        strlen(s) 
        + strlen(format) - 2 
        + 1
    );     
    sprintf(_str, format, s); 
    return _str;
}







/**
 * < : &lt;
 * & : &amp;
 */
char *html_escape(char *s){
    int extra_size = 0, 
        len = strlen(s);
    char *ret = NULL,
         *s2 = NULL,
         *s1 = s;

    while(*s1++){
        if('<' == *s1){
            extra_size += 3;
        }
        else if('&' == *s1){
            extra_size += 4;
        }
    }


    ret = s2 = (char *)malloc(len + extra_size + 1);
    s1 = s;

    /* 
    printf("escape target: %s\n", s1);
    */

    while(*s1){
        if('<' == *s1){
            strncpy(s2, "&lt;", 4);
            s2 += 4;
        }
        else if('&' == *s1){
            strncpy(s2, "&amp;", 5);
            s2 += 5;
        }
        else{
            *s2++ = *s1;
        }
        s1++;
    }
    *s2 = '\0';

    /* 
    printf("escape result: %s\n", ret);
    */
    return ret;
}


