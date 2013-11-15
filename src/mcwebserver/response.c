/**
 *  RESPONSE.C
 *  =========
 *  (c) Copyright Michael 2013 
 *  Email: hdm258i@gmail.com 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "response.h"
#include "helper.h"

void Response_Init(Response *resp){
    int i;

    for(i=0; i<20; i++){
        resp->headers[i] = NULL;
    }

    resp->body = (OutputBody *)calloc(sizeof(OutputBody), 1);
    OutputBody_Init(resp->body);
}



void OutputBody_Init(OutputBody *ob){
    ob->buffer = NULL;
    ob->length = 0;
    ob->size = 0;
}



void Response_Add_Header(Response *resp, char *header){
    int i;
    char *new_header;

    new_header = calloc(strlen(header) + 1, 1);

    strcpy(new_header, header);

    for(i=0; i<20; i++){
        if(!resp->headers[i]){
            resp->headers[i] = new_header;
            break;
        }
    }
}



void Response_Append_Body(Response *resp, char *body, size_t length){
    if(!body || !length){
        return;
    }

    if(resp->body->size == 0
        || resp->body->length + length > resp->body->size){

        resp->body->buffer 
            = realloc(
                resp->body->buffer
                , resp->body->size + OUTPUT_BUFFER_BLOCK_SIZE
            );

        resp->body->size += OUTPUT_BUFFER_BLOCK_SIZE;
    }

    /*
    strncpy(resp->body->buffer + resp->body->length, body, length);
    */

    /* Note: byte stream */
    memcpy(
        (void *)resp->body->buffer + resp->body->length
        , body
        , length
    );
    resp->body->length += length;
}



void Response_Output_Header(int conn, Response *resp){
    int i;
    char buffer[100];

    for(i=0; i<20; i++){
        if(resp->headers[i]){
            fprintf(
                stderr
                , "%s\r\n"
                , resp->headers[i]
            );
            sprintf(
                buffer
                , "%s\r\n"
                , resp->headers[i]
            );
            Writeline(conn, buffer, strlen(buffer));
        }
        else{
            break;
        }
    }
}


void Response_Output_Body(int conn, Response *resp){
    int i;

    fprintf(
        stderr
        , "body: [size: %d], [length: %d] \r\n"
        , (int)resp->body->size
        , (int)resp->body->length
    );
    Writeline(conn, resp->body->buffer, resp->body->length);
}



void Response_Output_All(int conn, Response *resp){
    Response_Output_Header(conn, resp);
    Writeline(conn, "\r\n", 2);
    Response_Output_Body(conn, resp);
}



void Response_OutputToFile_All(FILE *file, Response *resp){
    if(!file) {
        return;
    }

    Response_OutputToFile_Header(file, resp);
    Response_OutputToFile_Body(file, resp);
}


void Response_OutputToFile_Header(FILE *file, Response *resp){
    int i;
    char buffer[100];

    if(!file) {
        return;
    }

    for(i=0; i<20; i++){
        if(resp->headers[i]){
            sprintf(
                buffer
                , "%s\r\n"
                , resp->headers[i]
            );
            /* todo: error handle */
            fwrite((void *)buffer, 1, strlen(buffer), file);
        }
        else{
            break;
        }
    }
}


void Response_OutputToFile_Body(FILE *file, Response *resp){
    int i;

    if(!file) {
        return;
    }

    /* todo: error handle */
    fwrite(
        (void *)resp->body->buffer
        , 1
        , resp->body->length 
        , file
    );
}


void OutputBody_Free(OutputBody *ob){
    if(ob){
        if(ob->buffer){
            free(ob->buffer);
        }
    }
}



void Response_Free(Response *resp){
    int i; 

    if(!resp){
        return;
    }

    for(i=0; i<20; i++){
        if(resp->headers[i]){
            free(resp->headers[i]);
        }
    }

    OutputBody_Free(resp->body);
}


