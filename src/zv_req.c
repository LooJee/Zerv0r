#include <string.h>
#include <stdio.h>
#include "zv_req.h"
#include "zv_common.h"
#include "zv_hdr_func.h"
#include "zv_resp.h"

//static headCBs_t gHeadsHandles[] = {
//    {HEAD_HOST, hdrHostSet}
//};
//#define NUMBER_OF_HEADS (sizeof(gHeadsHandles)/sizeof(headCBs_t))

headSet getHeadParser(const char *s, const char *e)
{
    size_t len = e-s;
    headSet cb = hdrNonetSet;

    switch (len) {
        case 4: {
            if (strncmp(HEAD_HOST, s, len) == 0) {
                cb = hdrHostSet;
            } else {
                cb = NULL;
            }
        }
        default:
            break;
    }

    return cb;
}

ZV_PARSE_STATE_E zvParseHeads(pParseStruct_T p)
{
    p->state = ZV_PARSE_WAIT_HEAD;
    headSet headParseCb = NULL;
    for (char ch; p->state != ZV_PARSE_LINE_LF
                    && p->state != ZV_PARSE_ERROR
                    && p->state != ZV_PARSE_SUCCESS
                    && p->req_cursor != p->req_e; ++p->req_cursor) {
        ch = *p->req_cursor ;
        switch (p->state) {
            case ZV_PARSE_WAIT_HEAD: {
                if (isalnum(ch)) {
                    p->state = ZV_PARSE_HEAD_S;
                    p->p_cur_head_s = p->req_cursor;
                } else if (ch == SPC) {
                    break;
                } else if (ch == CR) {
                    p->state = ZV_PARSE_HEAD_CR;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_HEAD_S: {
                if (isalnum(ch) || ch == '-') {
                    break;
                } else if (ch == COLON || ch == SPC) {
                    p->p_cur_head_e = p->req_cursor;
                    headParseCb = getHeadParser(p->p_cur_head_s, p->p_cur_head_e);
                    p->state = ch == SPC ? ZV_PARSE_HEAD_E : ZV_PARSE_WAIT_VAL;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_HEAD_E: {
                if (ch == SPC) {
                    break;
                } else if (ch == COLON) {
                    p->state = ZV_PARSE_WAIT_VAL;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_WAIT_VAL: {
                headParseCb(p);
                if (p->state == ZV_PARSE_LINE_LF) {
                    --p->req_cursor;
                }
                break;
            }
            case ZV_PARSE_HEAD_CR: {
                if (ch == LF) {
                    p->state = ZV_PARSE_SUCCESS;
                    p->req->content = p->req_cursor+1;
                    --p->req_cursor;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            default: {
                p->state = ZV_PARSE_ERROR;
                break;
            }
        }
    }
    return p->state;
}


int zv_parseRequest(pParseStruct_T p)
{
    p->state = ZV_PARSE_IDLE;
    while (p->state != ZV_PARSE_ERROR && p->state != ZV_PARSE_SUCCESS) {
        switch (p->state) {
            case ZV_PARSE_IDLE:
            {
                p->state = hdrReqlineSet(p);
                break;
            }
            case ZV_PARSE_LINE_LF:
            {
                p->state = zvParseHeads(p);
                break;
            }
            default:
                p->state = ZV_PARSE_ERROR;
                break;
        }
    }

    return 0;
}