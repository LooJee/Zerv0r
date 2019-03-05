#include "zv_reqStruct.h"
#include "zv_common.h"

int hdrHostInit(pHdrHost_T *host)
{
    *host = (pHdrHost_T)malloc(sizeof(hdrHost_T));
    if (*host == NULL) {
        return -1;
    }
    (*host)->host_s = (*host)->host_e = NULL;
    (*host)->port = 0;

    return 0;
}

void hdrHostFree(pHdrHost_T *host)
{
    if (*host == NULL) {
        return;
    }

    ZV_S_FREE((*host));
}

ZV_PARSE_STATE_E hdrHostParse(pParseStruct_T p)
{
#define MAX_IP_SEC_NUM  255
#define MAX_PORT_NUM    65535
#define MAX_IP_SEC_CNT  3
    uint16_t ip_sec_num = 0;
    uint8_t  ip_sec_cnt = 0;
    for (char ch; p->state != ZV_PARSE_LINE_LF && p->state != ZV_PARSE_ERROR && p->req_cursor != p->req_e; ++p->req_cursor) {
        ch = *p->req_cursor;
        switch (p->state) {
            case ZV_PARSE_WAIT_VAL: {
                if (ch == SPC) {
                    break;
                } else if (isdigit(ch)) {
                    p->state = ZV_PARSE_IP_S;
                    p->req->host->host_s = p->req_cursor;
                    ip_sec_num = (ch - '0');
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_IP_S: {
                if (isdigit(ch)) {
                    ip_sec_num = (ip_sec_num * 10 + (ch-'0'));
                    if (ip_sec_num > MAX_IP_SEC_NUM) {
                        p->state = ZV_PARSE_ERROR;
                    }
                } else if (ch == DOT && ip_sec_cnt < MAX_IP_SEC_CNT && ip_sec_num <= MAX_IP_SEC_NUM) {
                    ++ip_sec_cnt;
                    ip_sec_num = 0;
                } else if (ch == COLON && ip_sec_cnt == MAX_IP_SEC_CNT && ip_sec_num <= MAX_IP_SEC_NUM) {
                    p->req->host->host_e = p->req_cursor;
                    p->state = ZV_PARSE_IP_E;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }

                break;
            }
            case ZV_PARSE_IP_E: {
                if (isdigit(ch)) {
                    p->req->host->port = (ch - '0');
                    p->state = ZV_PARSE_PORT_S;
                } else{
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_PORT_S: {
                if (isdigit(ch)) {
                    p->req->host->port = (p->req->host->port * 10 + (ch - '0'));
                    if (p->req->host->port > MAX_PORT_NUM) {
                        p->state = ZV_PARSE_ERROR;
                    }
                } else if (ch == SPC) {
                    p->state = ZV_PARSE_PORT_E;
                } else if (ch == CR) {
                    p->state = ZV_PARSE_LINE_CR;
                }
                break;
            }
            case ZV_PARSE_PORT_E: {
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

ZV_PARSE_STATE_E hdrHostSet(pParseStruct_T p)
{
    if (p == NULL) {
        return -1;
    }

    if (hdrHostInit(&(p->req->host)) == -1) {
        return -1;
    }

    return hdrHostParse(p);
}