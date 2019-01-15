#include <string.h>
#include <stdio.h>
#include "zv_req.h"

#define SKIP_SPACE(p) do { \
}while (p && *(++p) == ' ');

int zv_parseHead(const char *req, int size, pReqHead_S head)
{
#define MAX_HEADTYPE_SIZE 64
    printf("req : %s\n", req);

    char headType[MAX_HEADTYPE_SIZE] = {0};

    for (int i = 0; i < MAX_METHOD_SIZE && *req != ' '; ++i, ++req) {
        head->method[i] = *req;
    }

    SKIP_SPACE(req);
    
    for (int i = 0; i < MAX_URL_SIZE && *req != ' '; ++i, ++req) {
        head->url[i] = *req;
    }
    // ++req; //skip the character ' ' 
    SKIP_SPACE(req);

    for (int i = 0; i < MAX_VERSION_SIZE && *req != '\r'; ++i, ++req) {
        head->version[i] = *req;
    }
    req += 2; //skip "\r\n"

    printf("method : %s, url : %s, version : %s\n", head->method, head->url, head->version);

    //报文首部和报文正文以另起一行的\r\n来分隔
    while (*req != '\r' && *(req+1) != '\n') {
        char method[MAX_HEADTYPE_SIZE] = {0};
        int i = 0;
        while (*req != ':') {
            method[i++] = *(req++);
        }
        printf("method : %s\n", method);

        //skip one request method
        while (*req++ != '\n'){}
    }

    if (strncmp(head->method, "GET", strlen("GET")) == 0) {
        return 0;
    }
    else {
        return -1;
    }

    return 0;
}