#include "zv_reqStruct.h"
#include "zv_common.h"
#include "zv_hdr_func.h"
#include "zv_hdr_defs.h"
#include "zv_router_handlers.h"
#include <assert.h>
#include <ctype.h>

const char *g_httpMethods[] = {
        "GET",
        "POST"
};

const char *g_httpVersion[] = {
        "HTTP/0.9",
        "HTTP/1.0",
        "HTTP/1.1"
};

int hdrReqlineInit(pHdrReqLine_T *reqline)
{
    *reqline = (pHdrReqLine_T)malloc(sizeof(hdrReqLine_T));
    if (*reqline == NULL) {
        printf("malloc reqline failed\n");
        return -1;
    }
    (*reqline)->url = NULL;

    return 0;
}

void hdrReqlineFree(pHdrReqLine_T *reqline)
{
    if (*reqline == NULL) {
        return;
    }

    ZV_S_FREE((*reqline)->url);
    ZV_S_FREE((*reqline));
}

int setUri(pHdrReqLine_T req, const char *s, const char *e)
{
    size_t len = e-s+1;
    req->url = (char *)malloc(len);
    if (req->url == NULL) {
        printf("malloc url failed\n");
        return -1;
    }
    strncpy(req->url, s, len);
    req->url[len] = STR_EOF;

    return 0;
}

HTTP_VER_E setVer(pHdrReqLine_T req, const char *s, const char *e)
{
    req->version = HTTPMAX;
    size_t verSize = sizeof(g_httpVersion);
    size_t len = e-s+1;
    for (int i = 0; i < verSize; ++i) {
        if (strncmp(g_httpVersion[i], s, len) == 0) {
            req->version = (HTTP_VER_E)i;
            break;
        }
    }

    return req->version;
}

ZV_PARSE_STATE_E hdrReqlineParse(pParseStruct_T p)
{
    assert(p != NULL);
    assert(p->req_s != NULL);

    p->state = ZV_PARSE_WAIT_METHOD;
    for (char ch; p->state != ZV_PARSE_ERROR && p->state != ZV_PARSE_LINE_LF && p->req_cursor != p->req_e; ++p->req_cursor) {
        ch = *p->req_cursor;
        switch (p->state) {
            case ZV_PARSE_WAIT_METHOD: {
                if (ch == SPC || ch == CR || ch == LF) {
                    break;
                } else if (isupper(ch)) {
                    p->state = ZV_PARSE_METHOD_S;
                    p->p_method_s = p->req_cursor;
            } else {
            p->state = ZV_PARSE_ERROR;
        }
                break;
        }
        case ZV_PARSE_METHOD_S: {
            if (isupper(ch)) {
                break;
            } else if (ch == SPC) {
                p->p_method_e = p->req_cursor - 1;
                p->state = ZV_PARSE_METHOD_E;
            } else {
                p->state = ZV_PARSE_ERROR;
            }
            break;
        }
        case ZV_PARSE_METHOD_E: {
            if (ch == SLASH) {
                p->p_uri_s = p->req_cursor;
                p->state = ZV_PARSE_URI_S;
            } else if (ch == SPC) {
                break;
            } else {
                p->state = ZV_PARSE_ERROR;
            }
            break;
        }
        case ZV_PARSE_URI_S: {
            if (ch == SPC || ch == CR) {
                p->p_uri_e = p->req_cursor - 1;
                if ((p->handler = zvContainsUri(p->p_uri_s, p->p_uri_e)) != NULL) {
                    p->state = ch == SPC ? ZV_PARSE_URI_E : ZV_PARSE_LINE_CR;
                } else {
                    p->state = ZV_PARSE_ERROR;
                    p->ret = ZV_404;
                    p->handler = zvHttpExceptionHandler(p->ret);
                }
            } else if (isprint(ch)) {
                break;
            }
            break;
        }
        case ZV_PARSE_URI_E: {
            if (ch == SPC) {
                break;
            } else if (ch == CR) {
                p->state = ZV_PARSE_LINE_CR;
            } else if (ch == 'H') {
                p->state = ZV_PARSE_VER_S;
                p->p_ver_s = p->req_cursor;
            } else {
                p->state = ZV_PARSE_ERROR;
            }
            break;
            }
            case ZV_PARSE_VER_S: {
                if (isalnum(ch) || ch == SLASH || ch == DOT) {
                    break;
                } else if (ch == SPC || ch == CR) {
                    p->p_ver_e = p->req_cursor - 1;
                    if (setVer(p->req->reqline, p->p_ver_s, p->p_ver_e) != HTTPMAX) {
                        p->state = ch == SPC ? ZV_PARSE_VER_E : ZV_PARSE_LINE_CR;
                    } else {
                        p->state = ZV_PARSE_ERROR;
                    }
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_VER_E: {
                if (ch == SPC) {
                    break;
                } else if (ch == CR) {
                    p->state = ZV_PARSE_LINE_CR;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_LINE_CR: {
                if (ch == LF) {
                    p->state = ZV_PARSE_LINE_LF;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            default:
                p->state = ZV_PARSE_ERROR;
                break;
        }
    }

    return p->state;
}

ZV_PARSE_STATE_E hdrReqlineSet(pParseStruct_T p)
{
    if (p == NULL) {
        return -1;
    }

    if (hdrReqlineInit(&(p->req->reqline))) {
        return -1;
    }

    return hdrReqlineParse(p);
}
