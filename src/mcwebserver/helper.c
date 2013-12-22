/*

  HELPER.C
  ========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Implementation of helper functions for simple web server.
  The Readline() and Writeline() functions are shamelessly
  ripped from "UNIX Network Programming" by W Richard Stevens.

*/


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "conf.h"
#include "helper.h"


void SetEnv(const char *name, const char *value){
    char *val = "";

    /**
     * @note: value字段不能为NULL，否则设置不成功且退出程序，奇怪的是也捕获不到errno
     */
    if(-1 == setenv(name, value ? value : val, 1)){
        fprintf(stderr, "SetEnv: %s\n", strerror(errno));
    }
}

char * GetEnv(const char *name){
    return getenv(name);
}

/*  Prints an error message and quits  */

void Error_Quit(char const * msg) {
    fprintf(stderr, "WEBSERV: %s\n", msg);
    exit(EXIT_FAILURE);
}

char * GetFileExt(char const * name){
    char *ext = strrchr(name, '.');
    return ext ? ext : "";
}

char * GetFileName(char * path){
    char *name = strrchr(path, '/');
    if(!name 
        || name - path + 1 >= strlen(path)){
        name = path;
    }
    return name;
}

 
/*  Read a line from a socket  */

ssize_t Readline(
    int sockd, 
    void *vptr, 
    size_t maxlen) {

    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;
    /* initialize to an empty string */
    *buffer = 0;

    for ( n = 1; n < maxlen; n++ ) {
	
        if ( (rc = read(sockd, &c, 1)) == 1 ) {
            *buffer++ = c;
            if ( c == '\n' )
            break;
        }
        else if ( rc == 0 ) {
            if ( n == 1 )
                return 0;
            else
                break;
        }
        else {
            if ( errno == EINTR )
                continue;
            Error_Quit("Error in Readline()");
        }

    }

    *buffer = 0;
    return n;
}

ssize_t Readblock(
    int sockd, 
    void *vptr, 
    size_t maxlen) {

    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;
    /* initialize to an empty string */
    *buffer = 0;

    for ( n = 1; n < maxlen; n++ ) {
	
        if ( (rc = read(sockd, &c, 1)) == 1 ) {
            *buffer++ = c;
        }
        else if ( rc == 0 ) {
            if ( n == 1 )
                return 0;
            else
                break;
        }
        else {
            if ( errno == EINTR )
                continue;
            Error_Quit("Error in Readblock()");
        }

    }

    *buffer = 0;
    return n;
}

/*  Write a line to a socket  */

ssize_t Writeline(int sockd, const void *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
        if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
            if ( errno == EINTR )
                nwritten = 0;
            else
                Error_Quit("Error in Writeline()");
        }
        nleft  -= nwritten;
        buffer += nwritten;
    }

    return n;
}


/*  Removes trailing whitespace from a string  */

int Trim(char * buffer) {
    int n = strlen(buffer) - 1;

    /**
     * take into consideration：Accept:* / *
     */

    /*while ( !isalnum(buffer[n]) && n >= 0 )*/
    while ( isspace(buffer[n]) && n >= 0 )
	buffer[n--] = '\0';

    return 0;
}


/*  Converts a string to upper-case  */
    
int StrUpper(char * buffer) {
    while ( *buffer ) {
        *buffer = toupper(*buffer);
        ++buffer;
    }
    return 0;
}


/**
 * Cleans up url-encoded string  
 *     ASCII: %61%62%63 => abc 
 *            %E7%99%BE%E5%BA%A6 => 百度
 */
	
void CleanURL(char * buffer) {
    char asciinum[3] = {0};
    int i = 0, c;

    /*
    Console("Before CleanURL: %s", buffer);
    */
    
    while ( buffer[i] ) {
        if ( buffer[i] == '+' )
            buffer[i] = ' ';
        else if ( buffer[i] == '%' ) {
            asciinum[0] = buffer[i+1];
            asciinum[1] = buffer[i+2];
            buffer[i] = strtol(asciinum, NULL, 16);
            c = i+1;
            do {
                buffer[c] = buffer[c+2];
            } while ( buffer[2+(c++)] );
        }
        ++i;
    }

    /*
    Console("After CleanURL: %s", buffer);
    */
    
}

void Console(const char *fmt, ...){
    va_list arg_ptr;

    if(!Conf_Is_Debug()){
        return;
    }

    fprintf(stderr, "\t");

    va_start(arg_ptr, fmt);
    /* vfprintf not fprintf */
    vfprintf(stderr, fmt, arg_ptr); 
    va_end(arg_ptr);

    fprintf(stderr, "\n");
}




