/*

  REQHEAD.H
  =========
  (c) Copyright Paul Griffiths 1999
  Email: paulgriffiths@cwcom.net

  Interface to functions for manipulating HTTP request headers.

*/


#ifndef PG_REQHEAD_H
#define PG_REQHEAD_H


/*  User-defined data types  */

enum Req_Method { GET, HEAD, POST, UNSUPPORTED };
enum Req_Type   { SIMPLE, FULL };
enum CGI_Type   { NONE, PHP };

struct ReqInfo {
    enum Req_Method method;
    enum Req_Type   type;
    enum CGI_Type   cgi;
    char           *referer;
    char           *useragent;
    int            contentlength;

    char           *resource;
    char           *querystring;
    char           *body;

    int             status;
};


/*  Global macros/variables  */

#define MAX_REQ_LINE         (1024)
#define MAX_REQ_BODY         (1024 * 1024)


/*  Function prototypes  */

int  Parse_HTTP_Header(char * buffer, struct ReqInfo * reqinfo);
int  Get_Request      (int conn, struct ReqInfo * reqinfo);
void InitReqInfo      (struct ReqInfo * reqinfo);
void FreeReqInfo      (struct ReqInfo * reqinfo);

#endif  /*  PG_REQHEAD_H  */
