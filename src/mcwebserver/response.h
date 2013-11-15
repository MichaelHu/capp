/**
 *  RESPONSE.H
 *  =========
 *  (c) Copyright Michael 2013 
 *  Email: hdm258i@gmail.com 
 */

#ifndef MC_RESPONSE_H
#define MC_RESPONSE_H

#define OUTPUT_BUFFER_BLOCK_SIZE (1024 * 100)

typedef struct OutputBody {
    char    *buffer;
    size_t  length;
    size_t  size;
} OutputBody;

typedef struct Response {
    char            *headers[20];
    OutputBody      *body;
} Response;

void Response_Init(Response *resp);
void OutputBody_Init(OutputBody *ob);

void Response_Add_Header(Response *resp, char *header);
void Response_Append_Body(Response *resp, char *body, size_t length);

void Response_Output_All(int conn, Response *resp);
void Response_Output_Header(int conn, Response *resp);
void Response_Output_Body(int conn, Response *resp);

void Response_Free(Response *resp);
void OutputBody_Free(OutputBody *ob);


#endif
