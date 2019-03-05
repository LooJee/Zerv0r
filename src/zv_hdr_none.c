//
// Created by Zer0 on 2019/3/5.
//
#include "zv_reqStruct.h"

ZV_PARSE_STATE_E hdrNonetSet(pParseStruct_T p)
{
    for (char ch; p->state != ZV_PARSE_ERROR && p->state != ZV_PARSE_LINE_LF && p->req_cursor != p->req_e; ++p->req_cursor) {
        ch = *p->req_cursor;
        switch (p->state) {
            case ZV_PARSE_WAIT_VAL: {
                if (ch == CR) {
                    p->state = ZV_PARSE_LINE_CR;
                } else if (isprint(ch)) {
                    break;
                } else {
                    p->state = ZV_PARSE_ERROR;
                }
                break;
            }
            case ZV_PARSE_LINE_CR: {
                if (ch == LF) {
                    p->state = ZV_PARSE_LINE_LF;
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
