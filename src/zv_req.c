#include <string.h>
#include <stdio.h>
#include "zv_req.h"
#include "zv_common.h"
#include "zv_hdr_func.h"

static headCBs_t gHeadsHandles[] = {
    {HEAD_HOST, hdrHostSet}
};
#define NUMBER_OF_HEADS (sizeof(gHeadsHandles)/sizeof(headCBs_t))

int zv_callHeadCBs(pReqHead_S heads, const char * head,const char *values)
{
    for (int i = 0; i != NUMBER_OF_HEADS; ++i) {
        if (strncmp(head, gHeadsHandles[i].head, strlen(head)) == 0) {
            return gHeadsHandles[i].headCB(heads, values);
        }
    }

    return -2;
}

int zv_parseHead(const char *req, int size, pReqHead_S head)
{
#define MAX_HEADTYPE_SIZE 64
    printf("req : %s\n", req);

    int ret = hdrReqlineSet(head, req);
    if (ret == -1) {
        return -1;
    }

    printf("method : %s, url : %s, version : %s\n", head->reqline->method, head->reqline->url, head->reqline->version);
    req += ret;

    //报文首部和报文正文以另起一行的\r\n来分隔
    while (*req != '\r' && *(req+1) != '\n') {
        char method[MAX_HEADTYPE_SIZE] = {0};
        int i = 0;
        int offset = 0;
        while (*req != ':') {
            method[i++] = *(req++);
        }
        printf("method : %s\n", method);
        if ((offset = zv_callHeadCBs(head, method, req)) < 0) {
            SKIP_NEWLINE(req);
        } else {
            req += offset;
        }
    }

    if (strncmp(head->reqline->method, "GET", strlen("GET")) == 0) {
        return 0;
    } else if (strncmp(head->reqline->method, "POST", strlen("POST")) == 0) {
        return 0;
    } else {
        return -1;
    }

    return 0;
}