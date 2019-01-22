#ifndef _ZV_RESP_H_
#define _ZV_RESP_H_

#define ZV_HTTP_OK      200
#define ZV_HTTP_ERROR   404

typedef struct {
    int statusCode;
    int contentLength;
    char *contentType;
}respHead_T, *pRespHead_T; 

void respFile(int sock, const char *path);

#endif