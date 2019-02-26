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
    const char *s = value, *e = value;
    size_t len = 0;
    e = strchr(s, ' ');
    if (e == NULL) {
        return -1;
    }

    len = e-s;
    reqline->method = (char *)malloc(len+1);
    if (reqline->method == NULL) {
        printf("alloc method failed\n");
        goto ERROR_DEAL;
    }
    strncpy(reqline->method, s, len);
    reqline->method[len] = '\0';

    /*find slash*/
    while (*e != '\0' && *e != '/') {
        if (*e != ' ') {
            goto ERROR_DEAL;
        }
        ++e;
    }

    if (*e == '\0') {
        goto ERROR_DEAL;
    }

//    SKIP_SPACE(e);
    s = e;
    e = strchr(s, ' ');
    if (e == NULL) {
        goto ERROR_DEAL;
    }

    len = e-s;
    reqline->url = (char *)malloc(len+1);
    if (reqline->url == NULL) {
        printf("alloc url failed\n");
        goto ERROR_DEAL;
    }
    strncpy(reqline->url, s, len);
    reqline->url[len] = 0;

    while (*e == ' ') {
        ++e;
    }

    if (*e == '\0') {
        goto ERROR_DEAL;
    }

//    SKIP_SPACE(e);
    s = e;
    e = strchr(s, '\r');
    if (e == NULL) {
        goto ERROR_DEAL;
    }

    len = e-s;
    reqline->version = (char *)malloc(len+1);
    if (reqline->version == NULL) {
        printf("alloc version failed\n");
        hdrReqlineFree(&reqline);
        return -1;
    }
    strncpy(reqline->version, s, len);
    reqline->version[len] = 0;

    e = strchr(e, '\n');
    if (e == NULL) {
        goto ERROR_DEAL;
    }

    return e-value;

    ERROR_DEAL:
    hdrReqlineFree(&reqline);
    return -1;
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
