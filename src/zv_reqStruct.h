#ifndef _ZV_REQSTRUCT_H_
#define _ZV_REQSTRUCT_H_

#include "zv_hdr_defs.h"
#include "zv_common.h"
//#include "zv_router_handlers.h"

typedef struct {
    HTTP_METHOD_E method;
    HTTP_VER_E version;
    char *url;
}hdrReqLine_T, *pHdrReqLine_T;

typedef struct {
    char *host_s;
    char *host_e;
    int  port;
}hdrHost_T, *pHdrHost_T;

typedef struct {
    char *connType;
}hdrConn_T, *pHdrConn_T;

typedef enum {
    ZV_PARSE_IDLE,
    ZV_PARSE_WAIT_METHOD,
    ZV_PARSE_METHOD_S,
    ZV_PARSE_METHOD_E,
    ZV_PARSE_WAIT_URI,
    ZV_PARSE_URI_S,
    ZV_PARSE_URI_E,
    ZV_PARSE_WAIT_VER,
    ZV_PARSE_VER_S,
    ZV_PARSE_VER_E,
    ZV_PARSE_WAIT_HEAD,
    ZV_PARSE_HEAD_S,
    ZV_PARSE_HEAD_E,
    ZV_PARSE_WAIT_VAL,
    ZV_PARSE_VAL_S,
    ZV_PARSE_VAL_E,
    ZV_PARSE_IP_S,
    ZV_PARSE_IP_E,
    ZV_PARSE_PORT_S,
    ZV_PARSE_PORT_E,
    ZV_PARSE_LINE_CR,
    ZV_PARSE_LINE_LF,
    ZV_PARSE_HEAD_CR,
    ZV_PARSE_HEAD_LF,
    ZV_PARSE_ERROR,
    ZV_PARSE_SUCCESS,
    ZV_PARSE_DONE,
}ZV_PARSE_STATE_E;

typedef struct {
    pHdrReqLine_T       reqline;        //请求行
    pHdrHost_T          host;

    const char          *content;
}request_T, *pRequest_T;

/*请求处理函数指针*/
typedef void (*routerHandler)(int, pRequest_T);

typedef struct {
    pRequest_T          req;

    ZV_PARSE_STATE_E    state;          //http 请求解析状态
    ZV_HTTP_CODE_E      ret;            //http response 状态值
    routerHandler       handler;        //路由处理函数

    const char          *req_s;         //指向请求数据开始
    char                *req_cursor;    //指向请求数据解析位置
    char                *req_e;         //指向请求数据结束
    char                *p_method_s;    //指向 request method 开始指针
    char                *p_method_e;    //指向 request method 结束指针
    char                *p_uri_s;       //指向 request uri 开始指针
    char                *p_uri_e;       //指向 request uri 结束指针
    char                *p_ver_s;       //指向 request version 开始指针
    char                *p_ver_e;       //指向 request version 结束指针
    char                *p_cur_head_s;  //指向当前正在解析的 head 开始位置
    char                *p_cur_head_e;  //指向当前正在解析的 head 结束位置
}parseStruct_T, *pParseStruct_T;

/*
* @description: 函数指针，规定了暴露出来用于解析http首部字段的函数格式
*/
typedef ZV_PARSE_STATE_E (*headSet)(pParseStruct_T);

/*
* @description: 用户存放首部字段和与其对应的解析函数
*/
typedef struct {
    char *head;
    headSet headCB;
}headCBs_t, *pheadCBs_t;

#endif
