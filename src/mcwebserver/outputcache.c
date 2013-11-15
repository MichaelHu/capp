
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

#include "../md5/global.h"
#include "../md5/md5.h"

#include "outputcache.h"

int Is_Cachable(struct ReqInfo reqinfo){
    return 1;
}


int Is_Cached(struct ReqInfo reqinfo){
    return 1;
}


void Get_Cache_FilePath(char *file, struct ReqInfo reqinfo){
    size_t len = 0;
    unsigned char digest[16];

    char *str;
    MD5_CTX context;

    int i;

    if(reqinfo.resource){
        len = strlen(reqinfo.resource);
    }

    if(reqinfo.querystring){
        len += strlen(reqinfo.querystring);
    }

    if(len == 0){
        return;
    }

    str = (char *)calloc(len + 1, 1);
    
    if(reqinfo.resource){
        strcpy(str, reqinfo.resource);
    }

    if(reqinfo.querystring){
        strcpy(str + strlen(reqinfo.resource), reqinfo.querystring);
    }

    MD5Init( &context );
    MD5Update( &context, str, len ); 
    MD5Final (digest, &context);
    
    for(i=0; i<16; i++){
        sprintf(file + 2 * i, "%02x", digest[i]);
    }

    free(str);
}


void Write_Cache(char *file, Response resp){

}


void Get_Cache(char *file, Response *resp){

}
