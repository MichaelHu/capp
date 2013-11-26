
/**
 *  OUTPUTCACHE.C
 *  =========
 *  (c) Copyright Michael 2013 
 *  Email: hdm258i@gmail.com 
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../md5/global.h"  /* for UINT4 */
#include "../md5/md5.h"     /* for MD5String */

#include "helper.h"
#include "outputcache.h"
#include "response.h"

int Is_Cachable(struct ReqInfo reqinfo){
    return reqinfo.cgi != NONE;
}


int Is_Cached(struct ReqInfo reqinfo){
    char cachefile[64] = {0};
    FILE *cache_file;

    Get_Cache_FilePath(cachefile, reqinfo);

    if( ( cache_file = fopen(cachefile, "r") ) ){
        /* note: must close the file */
        fclose(cache_file);
        return 1;
    }    
    return 0;
}


void Get_Cache_FilePath(char *file, struct ReqInfo reqinfo){
    size_t len = 0;
    char *str, md5_str[33] = {0};

    if(reqinfo.originalresource){
        len = strlen(reqinfo.originalresource);
    }

    if(reqinfo.querystring){
        len += strlen(reqinfo.querystring);
    }

    if(reqinfo.body){
        len += strlen(reqinfo.body);
    }

    if(len == 0){
        return;
    }

    str = (char *)calloc(len + 1, 1);
    
    if(reqinfo.originalresource){
        strcpy(str, reqinfo.originalresource);
    }

    if(reqinfo.querystring){
        strcpy(str + strlen(reqinfo.originalresource)
            , reqinfo.querystring);
    }

    if(reqinfo.body){
        strcpy(str + strlen(reqinfo.originalresource) 
                + strlen(reqinfo.querystring)
            , reqinfo.body);
    }

    MD5String(str, md5_str);

    sprintf(file, "./cache/%s", md5_str);

    free(str);
}


void Write_Cache(char *file, Response resp){
    FILE *cache_file;

    if(Response_Is_CGI_Fail(&resp)){
        return;
    }
    
    if( !(cache_file = fopen(file, "w") ) ){
        return;
    }

    Console("Write cache: %s", file);
    Response_OutputToFile_All(cache_file, &resp);

    Console("File Pos: %ld", ftell(cache_file));
    fclose(cache_file);
}


void Get_Cache(char *file, Response *resp){
    FILE *cache_file;
    int len = 0;
    char buffer[CACHE_BUF_SIZE];
    
    if( !(cache_file = fopen(file, "r") ) ){
        return;
    }
   
    while( (len = fread(buffer, 1, CACHE_BUF_SIZE, cache_file) ) > 0 ){
        if(ferror(cache_file)){
            Error_Quit("Error reading from cache file.");
        }
        else{
            Response_Append_Body(resp, buffer, len);
        }
            
        if(feof(cache_file)){
            break;
        }
    } 

}

