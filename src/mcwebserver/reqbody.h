#ifndef PG_REQBODY_H
#define PG_REQBODY_H

/*  Global macros/variables  */

#define MAX_REQ_BODY         (1024 * 1024)

/*  Function prototypes  */

int  Get_Request_Body (int conn, struct ReqInfo * reqinfo, int maxlen);

#endif  /*  PG_REQHEAD_H  */

