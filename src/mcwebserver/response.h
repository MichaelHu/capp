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

int Response_Is_CGI_Fail(Response *resp);
void Response_Add_Header(Response *resp, char *header);
void Response_Append_Body(Response *resp, char *body, size_t length);

/* Parse headers and body from byte stream */
void Response_Parse_From_Stream(char *buf, int length, Response *resp);

void Response_Output_All(int conn, Response *resp);
void Response_Output_Header(int conn, Response *resp);
void Response_Output_Body(int conn, Response *resp);

void Response_OutputToFile_All(FILE *file, Response *resp);
void Response_OutputToFile_Header(FILE *file, Response *resp);
void Response_OutputToFile_Body(FILE *file, Response *resp);

void Response_Free(Response *resp);
void OutputBody_Free(OutputBody *ob);


#endif
