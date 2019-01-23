//
// Created by Zer0 on 2019/1/23.
//

#ifndef ZERV0R_ZV_ROUTER_HANDLERS_H
#define ZERV0R_ZV_ROUTER_HANDLERS_H

#include "zv_reqStruct.h"

typedef void (*routerHandler)(int, pReqHead_S);

typedef struct {
    char *router;
    routerHandler handler;
}routers_T, pRouters_T;

void zvHandleRouters(int sock, pReqHead_S head);

#endif //ZERV0R_ZV_ROUTER_HANDLERS_H
