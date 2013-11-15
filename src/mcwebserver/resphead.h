/*

  RESPHEAD.H
  ==========
  (c) Copyright Paul Griffiths 1999
  Email: paulgriffiths@cwcom.net

  Interface to HTTP response header functions

*/


#ifndef PG_RESPHEAD_H
#define PG_RESPHEAD_H

#include "response.h"
#include "reqhead.h"          /*  for struct ReqInfo  */


/*  Function prototypes  */

int Output_HTTP_Headers(int conn, struct ReqInfo * reqinfo, Response *resp);


#endif  /*  PG_RESPHEAD_H  */
