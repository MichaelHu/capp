/*

  SERVREQ.C
  =========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Implementation of function to service requests.

*/


/* setenv, unsetenv */
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "helper.h"
#include "reqhead.h"
#include "resphead.h"
#include "resource.h"

void ProcessPHP(int conn, struct ReqInfo reqinfo);

/*  Service an HTTP request  */

int Service_Request(int conn) {

    struct ReqInfo  reqinfo;
    int             resource = 0;


    InitReqInfo(&reqinfo);
    
    /*  Get HTTP request  */
    if ( Get_Request(conn, &reqinfo) < 0 ){
        return -1;
    }
    
    /**
     * Check whether resource exists, whether we have permission
	 * to access it, and update status code accordingly.          
     */
    if ( reqinfo.status == 200 ){
        if ( (resource = Check_Resource(&reqinfo)) < 0 ) {
            if ( errno == EACCES )
                reqinfo.status = 401;
            else
                reqinfo.status = 404;
        }
    }

    /*  Output HTTP response headers if we have a full request  */
    if ( reqinfo.type == FULL ){
        Output_HTTP_Headers(conn, &reqinfo);
    }

    /*  Service the HTTP request  */
    if ( reqinfo.status == 200 ) {
        if(resource > 0){
            if(reqinfo.cgi == PHP){
                ProcessPHP(conn, reqinfo);
            }  
            else if( Return_Resource(conn, resource, &reqinfo) ){
                Error_Quit("Something wrong returning resource.");
            }
            
        }
        else{
            /* 404 */
        }
    }
    else {
        Return_Error_Msg(conn, &reqinfo);
    }

    if ( resource > 0 ){
        if ( close(resource) < 0 ){
            Error_Quit("Error closing resource.");
        }
    }

    FreeReqInfo(&reqinfo);

    return 0;
}

void ProcessPHP(int conn, struct ReqInfo reqinfo){
    /*
    FILE *fp;
    */
    char output[10000] = {0};
    int pfd1[2], pfd2[2];
    pid_t pid;
    char c;

    /**
     * 使用双管道达到父子进程间双工通信
     */
    if(pipe(pfd1) < 0 || pipe(pfd2) < 0){
        Error_Quit("Can not open pipe.");
    }

    SetEnv("DOCUMENT_ROOT", GetDocRoot());
    SetEnv("QUERY_STRING", reqinfo.querystring);

    memset(output, 0, 10000);
    sprintf(output, "%d", reqinfo.status);
    SetEnv("REDIRECT_STATUS", output);

    memset(output, 0, 10000);
    sprintf(output, "%s/%s", GetDocRoot(), reqinfo.resource);
    SetEnv("SCRIPT_FILENAME", output);

    if(reqinfo.method == GET){
        SetEnv("REQUEST_METHOD", "GET");
    }
    else{
        SetEnv("REQUEST_METHOD", "POST");
        SetEnv("CONTENT_TYPE", "application/x-www-form-urlencoded");

        memset(output, 0, 10000);
        sprintf(output, "%d", (int)strlen(reqinfo.querystring));
        SetEnv("CONTENT_LENGTH", output);
    }

    if((pid = fork()) < 0){
        Error_Quit("Can not fork.");
    }
    else if(pid > 0){/* parent */

        close(pfd1[0]);
        close(pfd2[1]);

        /** 
         * 子进程从标准输入中获取POST数据
         */
        if(reqinfo.querystring
            && write(
                pfd1[1], 
                reqinfo.querystring, 
                strlen(reqinfo.querystring)
            ) 
            != strlen(reqinfo.querystring)){
            Error_Quit("Write Error");
        }

        while(read(pfd2[0], &c, 1) > 0){
            fprintf(stderr, "%c", c);
            Writeline(conn, &c, 1);
        }

        if(waitpid(pid, NULL, 0) < 0){
            Error_Quit("waitpid error.");
        }

    }
    else{/* child */

        close(pfd1[1]);
        close(pfd2[0]);

        if(pfd1[0] != STDIN_FILENO){
            if(dup2(pfd1[0], STDIN_FILENO) != STDIN_FILENO){
                Error_Quit("Can not duplicate to STDIN");
            }
            close(pfd1[0]);
        }

        if(pfd2[1] != STDOUT_FILENO){
            if(dup2(pfd2[1], STDOUT_FILENO) != STDOUT_FILENO){
                Error_Quit("Can not duplicate to STDOUT");
            }
            close(pfd2[1]);
        }

        /*
        fgets(output, 10, stdin);
        puts("output: ");
        puts(output);

        printf("/usr/bin/php test.php");
        */

        if(execl("/usr/bin/php-cgi", "php-cgi", (char *)0 ) < 0){
            Error_Quit("Error in execl");
        }

    }


    /*
    setenv("QUERY_STRING", reqinfo.querystring, 1);
    fp = popen("php test.php", "r");
    unsetenv("QUERY_STRING");

    while(fgets(output, 10000, fp) != NULL){
        fprintf(stderr, "= %s", output);
        memset(output, 0, 100);
    }
    */

}


