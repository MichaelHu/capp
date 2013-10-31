/*

  RESPHEAD.C
  ==========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Implementation of HTTP reponse header functions.

*/


#include <unistd.h>

#include <string.h>
#include <stdio.h>

#include "resphead.h"
#include "helper.h"

char *Get_MIME_Type(char *name){
    char *ext = strrchr(name, '.');

    if (!ext) return NULL;
    if (strcmp(ext, ".html") == 0 
        || strcmp(ext, ".htm") == 0
        || strcmp(ext, ".tpl") == 0) return "text/html";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "text/javascript";
    if (strcmp(ext, ".au") == 0) return "audio/basic";
    if (strcmp(ext, ".wav") == 0) return "audio/wav";
    if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
    if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";
    if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";


    /* for .apk */
    if (strcmp(ext, ".apk") == 0) return "application/vnd.android.package-archive";

    /*return NULL;*/
    return "text/plain";
}

char *Get_FileName(char *name){
    char *ext = strrchr(name, '/');
    if(!ext || *(ext + 1) == '\0'){
        return "";
    }
    return ext + 1;
}

/*  Outputs HTTP response headers  */

int Output_HTTP_Headers(int conn, struct ReqInfo * reqinfo) {

    char buffer[100], *mime;

    sprintf(buffer, "HTTP/1.0 %d OK\r\n", reqinfo->status);
    Writeline(conn, buffer, strlen(buffer));

    Writeline(conn, "Server: PGWebServ v0.1\r\n", 24);

    if(reqinfo->cgi == NONE){

        if(reqinfo->status == 200){
            mime = Get_MIME_Type(reqinfo->resource);
            memset(buffer, 0, strlen(buffer));
            sprintf(buffer, "Content-Type: %s\r\n", mime);
            Writeline(conn, buffer, strlen(buffer));

            /* for .apk archive */
            if(strcmp(mime, "application/vnd.android.package-archive") == 0){
                memset(buffer, 0, strlen(buffer));
                sprintf(buffer
                    , "Content-Disposition: attachment;filename=%s\r\n"
                    , Get_FileName(reqinfo->resource));
                Writeline(conn, buffer, strlen(buffer));
            }
        }
        else{
            Writeline(conn, "Content-Type: text/html\r\n", 25);
        }

        Writeline(conn, "\r\n", 2);

    }

    return 0;
}



