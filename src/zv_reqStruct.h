#ifndef _ZV_REQSTRUCT_H_
#define _ZV_REQSTRUCT_H_

#include "zv_hdr_defs.h"

typedef struct {
    char *method;
    char *url;
    char *version;
}hdrReqLine_T, *pHdrReqLine_T;

typedef struct {
    char *host;
    char *port;
}hdrHost_T, *pHdrHost_T;

typedef struct {
    char *connType;
}hdrConn_T, *pHdrConn_T;

typedef struct {
    pHdrReqLine_T   reqline;
    pHdrHost_T      host;
}reqHead_S, *pReqHead_S;

/*
* @description: 函数指针，规定了暴露出来用于设置http首部字段的函数格式
*/
typedef int (*headSet)(pReqHead_S, const char *);

/*
* @description: 用户存放首部字段和与其对应的设置函数
*/
typedef struct {
    char *head;
    headSet headCB;
}headCBs_t, *pheadCBs_t;


#endif
