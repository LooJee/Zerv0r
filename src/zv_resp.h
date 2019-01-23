#ifndef _ZV_RESP_H_
#define _ZV_RESP_H_

#include "zv_common.h"

#define MAX_RESP_HEAD_SIZE 1024

typedef enum {
    ZV_HTTP_OK = 200,
    ZV_HTTP_NOT_FOUND = 404,
    ZV_HTTP_MAX
}ZV_HTTP_STATUS_E;

typedef struct {
    ZV_HTTP_STATUS_E statusCode;
    char *statusStr;
    size_t contentLength;
    char *contentType;
}respHead_T, *pRespHead_T; 

typedef struct {
    pRespHead_T head;
    char *content;
}resp_T, *pResp_T;

void respInit();
int respText(int sock, ZV_HTTP_STATUS_E status, const char *text);
int respHTMLFile(int sock, ZV_HTTP_STATUS_E status, const char *path);
int respHTMLText(int sock, ZV_HTTP_STATUS_E status, const char *text);

#endif