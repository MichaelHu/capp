/*

  RESOURCE.C
  ==========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Implementation of functions for returning a resource.

*/


#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>

#include "resource.h"
#include "reqhead.h"
#include "helper.h"

#define RES_BUF_SIZE (4096)

/*  Change this string to change the root directory that
    the server will use, i.e. /index.html will translate
    here to /home/httpd/html/index.html                   */

/*
static char server_root[1000] = "/home/httpd/html";
*/
static char server_root[500] = ".";
static char resource_path[1000] = "";

void SetDocRoot(char *path){
    memset(server_root, 0, strlen(server_root)); 
    strcpy(server_root, path);
    fprintf(stderr, "SetDocRoot: %s\n", path);
}

char *GetDocRoot(){
    return server_root;
}

/*  Returns a resource  */

int Return_Resource(int conn, int resource, struct ReqInfo * reqinfo, Response * resp) {

    int  i;

    /*
    char c;
    while ( (i = read(resource, &c, 1)) ) {
        if ( i < 0 )
            Error_Quit("Error reading from file.");
        if ( write(conn, &c, 1) < 1 )
            Error_Quit("Error sending file.");
    }
    */

    char buffer[RES_BUF_SIZE];
    while((i = read(resource, buffer, RES_BUF_SIZE)) > 0){
        if(i < 0)
            Error_Quit("Error reading from file.");

        /*
        if ( write(conn, buffer, i) < i )
            Error_Quit("Error sending file.");
        */

        Response_Append_Body(resp, buffer, i);

        if(i < RES_BUF_SIZE){
            break;
        }
    }

    return 0;
}


/*  Tries to open a resource. The calling function can use
    the return value to check for success, and then examine
    errno to determine the cause of failure if neceesary.    */

int Check_Resource(struct ReqInfo * reqinfo) {

    /*  Resource name can contain urlencoded
	data, so clean it up just in case.    */

    CleanURL(reqinfo->resource);

    
    /*  Concatenate resource name to server root, and try to open  */

    sprintf(resource_path, "%s/%s", server_root, reqinfo->resource);
    return open(resource_path, O_RDONLY);
}


/*  Returns an error message  */

int Return_Error_Msg(int conn, struct ReqInfo * reqinfo) {
    
    char buffer[100];

    sprintf(buffer, "<HTML>\n<HEAD>\n<TITLE>Server Error %d</TITLE>\n"
	            "</HEAD>\n\n", reqinfo->status);
    Writeline(conn, buffer, strlen(buffer));

    sprintf(buffer, "<BODY>\n<H1>Server Error %d</H1>\n", reqinfo->status);
    Writeline(conn, buffer, strlen(buffer));

    sprintf(buffer, "<P>The request could not be completed.</P>\n"
	            "</BODY>\n</HTML>\n");
    Writeline(conn, buffer, strlen(buffer));

    return 0;

}
