/**
 *  OUTPUTCACHE.H
 *  =========
 *  (c) Copyright Michael 2013 
 *  Email: hdm258i@gmail.com 
 */

#ifndef MC_OUTPUTCACHE_H
#define MC_OUTPUTCACHE_H

#include "reqhead.h"
#include "response.h"

#define CACHE_BUF_SIZE (64)

int Is_Cachable(struct ReqInfo reqinfo);
int Is_Cached(struct ReqInfo reqinfo);

void Get_Cache_FilePath(char *file, struct ReqInfo reqinfo);
void Write_Cache(char *file, Response resp);
void Get_Cache(char *file, Response *resp);

#endif

