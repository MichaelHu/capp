#include <sys/time.h>             /*  For select()  */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "reqhead.h"
#include "reqbody.h"
#include "servreq.h"
#include "helper.h"

int Get_Request_Body(int conn, struct ReqInfo * reqinfo, int maxlen) {

    char   buffer[MAX_REQ_BODY] = {0};
    int    rval;
    fd_set fds;
    struct timeval tv;

    char   *body = NULL;
    int    left = maxlen, len = 0;

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
            Error_Quit("Error calling select() in get_request_body()");
        }
        else if ( rval == 0 ) {

            /*  input not ready after timeout  */

            return -1;

        }
        else {
            /*  We have an input line waiting, so retrieve it  */

            /* todo: when len > MAX_REQ_BODY */
            len = Readblock(conn, buffer, left + 1/* max buffer size, including terminated character*/);

            if(len == 0){
                break;
            }

            left -= len;

            /*
            fprintf(stderr, "buffer: %s\n", buffer);
            */

            len = strlen(buffer);
            if(body){
                body = realloc(body, strlen(body) + len + 1);
                strcpy(body + strlen(body) + 1, buffer);
            }
            else{
                body = calloc(len + 1, 1);
                strcpy(body, buffer);
                *(body + len) = 0;
            }

        }

    } while ( left > 0 );

    fprintf(stderr, "    body: %s\n", body);
    reqinfo->body = body;

    return 0;
}



