#include "zv_reqStruct.h"
#include "zv_common.h"
#include "zv_hdr_func.h"

int hdrReqlineInit(pHdrReqLine_T *reqline)
{
    *reqline = (pHdrReqLine_T)malloc(sizeof(hdrReqLine_T));
    if (*reqline == NULL) {
        printf("malloc reqline failed\n");
        return -1;
    }
    (*reqline)->method = NULL;
    (*reqline)->url = NULL;
    (*reqline)->version = NULL;

    return 0;
}

void hdrReqlineFree(pHdrReqLine_T *reqline)
{
    if (*reqline == NULL) {
        return;
    }

    S_FREE((*reqline)->method);
    S_FREE((*reqline)->url);
    S_FREE((*reqline)->version);

    S_FREE((*reqline));
}

int hdrReqlineParse(pHdrReqLine_T reqline, const char *value)
{
    char *s = value, *e = value;
    e = strchr(s, ' ');
    reqline->method = (char *)malloc(e-s);
    if (reqline->method == NULL) {
        printf("alloc method failed\n");
        hdrReqlineFree(&reqline);
        return -1;
    }
    strncpy(reqline->method, s, e-s);

    SKIP_SPACE(e);
    s = e;
    e = strchr(s, ' ');
    reqline->url = (char *)malloc(e-s);
    if (reqline->url == NULL) {
        printf("alloc url failed\n");
        hdrReqlineFree(&reqline);
        return -1;
    }
    strncpy(reqline->url, s, e-s);

    SKIP_SPACE(e);
    s = e;
    e = strchr(s, '\r');
    reqline->version = (char *)malloc(e-s);
    if (reqline->version == NULL) {
        printf("alloc version failed\n");
        hdrReqlineFree(&reqline);
        return -1;
    }
    strncpy(reqline->version, s, e-s);

    e = strchr(e, '\n');
    SKIP_NEWLINE(e);

    return e-value;
}

int hdrReqlineSet(pReqHead_S head, const char *value)
{
    if (head == NULL) {
        return -1;
    }

    if (hdrReqlineInit(&(head->reqline))) {
        return -1;
    }

    return hdrReqlineParse(head->reqline, value);
}
