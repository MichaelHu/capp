/*

  REQHEAD.C
  =========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Implementation of functions to manipulate HTTP request headers.

*/


#include <sys/time.h>             /*  For select()  */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "reqhead.h"
#include "reqbody.h"
#include "servreq.h"
#include "helper.h"


/*  Parses a string and updates a request
    information structure if necessary.    */

int Parse_HTTP_Header(char * buffer, struct ReqInfo * reqinfo) {

    static int first_header = 1;
    char      *temp;
    char      *endptr;
    int        len, has_qs = 1;

    if ( first_header == 1 ) {

        /*  If first_header is 0, this is the first line of
            the HTTP request, so this should be the request line.  */


        /*  Get the request method, which is case-sensitive. This
            version of the server only supports the GET and HEAD
            request methods.                                        */

        if ( !strncmp(buffer, "GET ", 4) ) {
            reqinfo->method = GET;
            fprintf(stderr, "\n> %s\n", buffer);
            buffer += 4;
        }
        else if ( !strncmp(buffer, "HEAD ", 5) ) {
            reqinfo->method = HEAD;
            buffer += 5;
        }
        else if ( !strncmp(buffer, "POST ", 5) ) {
            reqinfo->method = POST;
            fprintf(stderr, "\n> %s\n", buffer);
            buffer += 5;
        }
        else {
            reqinfo->method = UNSUPPORTED;
            reqinfo->status = 501;
            return -1;
        }


        /*  Skip to start of resource  */

        while ( *buffer && isspace(*buffer) )
            buffer++;


        /*  Calculate string length of resource...  */

        endptr = strchr(buffer, '?');
        if(NULL == endptr){
            has_qs = 0;
            endptr = strchr(buffer, ' ');
        }

        if ( endptr == NULL ){
            len = strlen(buffer);
        } 
        else{
            len = endptr - buffer;
        }

        if ( len == 0 ) {
            reqinfo->status = 400;
            return -1;
        }

        /*  ...and store it in the request information structure.  */

        reqinfo->resource = calloc(len + 1, sizeof(char));
        reqinfo->originalresource = reqinfo->resource;

        strncpy(reqinfo->resource, buffer, len);
        CleanURL(reqinfo->resource);

        /* strcmp(s1, s2): note!! s1 or s2 can not be NULL  */
        if(0 == strcmp(GetFileExt(reqinfo->resource), ".php")){
            reqinfo->cgi = PHP;
        }

        if(has_qs){
            /* skip ? */
            buffer = endptr + 1;
            endptr = strchr(buffer, ' ');

            if ( endptr == NULL ){
                len = strlen(buffer);
            } 
            else{
                len = endptr - buffer;
            }

            reqinfo->querystring = calloc(len + 1, sizeof(char));
            strncpy(reqinfo->querystring, buffer, len);
        }

        /*  Test to see if we have any HTTP version information.
            If there isn't, this is a simple HTTP request, and we
            should not try to read any more headers. For simplicity,
            we don't bother checking the validity of the HTTP version
            information supplied - we just assume that if it is
            supplied, then it's a full request.                        */

        if ( strstr(buffer, "HTTP/") )
            reqinfo->type = FULL;
        else
            reqinfo->type = SIMPLE;

        first_header = 0;

        return 0;
    }

    /*  If we get here, we have further headers aside from the
	request line to parse, so this is a "full" HTTP request.  */

    /*  HTTP field names are case-insensitive, so make an
	upper-case copy of the field name to aid comparison.
	We need to make a copy of the header up until the colon.
	If there is no colon, we return a status code of 400
	(bad request) and terminate the connection. Note that
	HTTP/1.0 allows (but discourages) headers to span multiple
	lines if the following lines start with a space or a
	tab. For simplicity, we do not allow this here.              */

    endptr = strchr(buffer, ':');
    if ( endptr == NULL ) {
        reqinfo->status = 400;
        return -1;
    }

    temp = calloc( (endptr - buffer) + 1, sizeof(char) );
    strncpy(temp, buffer, (endptr - buffer));
    StrUpper(temp);


    /*  Increment buffer so that it now points to the value.
	If there is no value, just return.                    */

    buffer = endptr + 1;

    while ( *buffer && isspace(*buffer) )
        ++buffer;

    if ( *buffer == '\0' )
     	return 0;


    /*  Now update the request information structure with the
	appropriate field value. This version only supports the
	"Referer:" and "User-Agent:" headers, ignoring all others.  */

    if ( !strcmp(temp, "USER-AGENT") ) {
	    reqinfo->useragent = malloc( strlen(buffer) + 1 );
	    strcpy(reqinfo->useragent, buffer);
    }
    else if ( !strcmp(temp, "REFERER") ) {
	    reqinfo->referer = malloc( strlen(buffer) + 1 );
	    strcpy(reqinfo->referer, buffer);
    }
    else if ( !strcmp(temp, "CONTENT-LENGTH") ) {
        sscanf(buffer, "%d", &reqinfo->contentlength);
    }

    free(temp);
    return 0;
}


/*  Gets request headers and body if any. A CRLF terminates a HTTP header line,
    but if one is never sent we would wait forever. Therefore,
    we use select() to set a maximum length of time we will
    wait for the next complete header. If we timeout before
    this is received, we terminate the connection.               */

int Get_Request(int conn, struct ReqInfo * reqinfo) {

    char   buffer[MAX_REQ_LINE] = {0};
    int    rval;
    fd_set fds;
    struct timeval tv;


    /*  Set timeout to 5 seconds  */

    tv.tv_sec  = 5;
    tv.tv_usec = 0;


    /*  Loop through request headers. If we have a simple request,
	then we will loop only once. Otherwise, we will loop until
	we receive a blank line which signifies the end of the headers,
	or until select() times out, whichever is sooner.                */

    do {

        /*  Reset file descriptor set  */

        FD_ZERO(&fds);
        FD_SET (conn, &fds);


        /*  Wait until the timeout to see if input is ready  */
        rval = select(conn + 1, &fds, NULL, NULL, &tv);


        /*  Take appropriate action based on return from select()  */

        if ( rval < 0 ) {
            Error_Quit("Error calling select() in get_request()");
        }
        else if ( rval == 0 ) {

            /*  input not ready after timeout  */

            return -1;

        }
        else {
            /*  We have an input line waiting, so retrieve it  */
            Readline(conn, buffer, MAX_REQ_LINE - 1);
            Trim(buffer);

            if ( buffer[0] == '\0' ){
                break;
            }

            if ( Parse_HTTP_Header(buffer, reqinfo) ){
                fprintf(stderr, "Error in Parse_HTTP_Header\n");
                break;
            }
        }

    } while ( reqinfo->type != SIMPLE );

    /* get request body if any */
    if(reqinfo->contentlength){
        fprintf(stderr, "  Request Content-Length: %d\n", reqinfo->contentlength);
        Get_Request_Body(conn, reqinfo, reqinfo->contentlength);
    }

    return 0;
}

/*  Initialises a request information structure  */

void InitReqInfo(struct ReqInfo * reqinfo) {
    reqinfo->useragent = NULL;
    reqinfo->referer   = NULL;
    reqinfo->resource  = NULL;
    reqinfo->originalresource  = NULL;
    reqinfo->querystring  = NULL;
    reqinfo->contentlength  = 0;
    reqinfo->body  = NULL;
    reqinfo->method    = UNSUPPORTED;
    reqinfo->status    = 200;          
    reqinfo->cgi    = NONE;          
}

void PrintReqInfo(struct ReqInfo * reqinfo){

    fprintf(stderr
        ,"  Request UA: %s\n"
        ,reqinfo->useragent ? reqinfo->useragent : "");    

    /*
    fprintf(stderr
        ,"\tReferer: %s\n"
        ,reqinfo->referer ? reqinfo->referer : "");    

    Console("Request Resource: %s"
        , reqinfo->resource ? reqinfo->resource : "");    

    Console("Request QueryString: %s"
        ,reqinfo->querystring ? reqinfo->querystring : "");    

    */
}

/*  Frees memory allocated for a request information structure  */

void FreeReqInfo(struct ReqInfo * reqinfo) {
    if ( reqinfo->useragent )
        free(reqinfo->useragent);

    if ( reqinfo->referer )
        free(reqinfo->referer);

    if ( reqinfo->resource == reqinfo->originalresource){
        free(reqinfo->resource);
    }
    else{
        free(reqinfo->resource);
        free(reqinfo->originalresource);
    }


    if ( reqinfo->querystring )
        free(reqinfo->querystring);

    if ( reqinfo->body )
        free(reqinfo->body);
}
