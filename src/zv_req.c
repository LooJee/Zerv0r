#include <string.h>
#include <stdio.h>
#include "zv_req.h"

int zv_parseHead(const char *req, int size, pReqHead_S head)
{
#define MAX_HEADTYPE_SIZE 64

    char headType[MAX_HEADTYPE_SIZE] = {0};

    for (int i = 0; i < MAX_METHOD_SIZE && *req != ' '; ++i, ++req) {
        head->method[i] = *req;
    }
    ++req; //skip the character ' '
    
    for (int i = 0; i < MAX_URL_SIZE && *req != ' '; ++i, ++req) {
        head->url[i] = *req;
    }
    ++req; //skip the character ' ' 

    for (int i = 0; i < MAX_VERSION_SIZE && *req != '\r'; ++i, ++req) {
        head->version[i] = *req;
    }
    req += 2; //skip "\r\n"

    while (*req != '\0') {
        int i = 0;
        for (i = 0; i < MAX_HEADTYPE_SIZE && *req != ':'; ++i, ++req) {
            headType[i] = *req;
        }
        headType[i] = '\0';
        printf("head type : %s\n", headType);
        if (strcmp(headType, "Connection") == 0) {
            while (*(req++) == ' ') {}

            for (int i = 0; i < MAX_CONNECTION_SIZE && *req != '\r'; ++i, ++req) {
                head->Connection[i] = *req;
            }

            break;
        }

        while (*(req++) != '\n'){}
    }

    printf("method : %s, url : %s, version : %s, Connection : %s\n", head->method, head->url, head->version, head->Connection);

    if (strncmp(head->method, "GET", strlen("GET")) == 0) {
        return 0;
    }
    else {
        return -1;
    }

    return 0;
}