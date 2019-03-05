//
// Created by Zer0 on 2019/1/23.
//

#ifndef ZERV0R_ZV_ROUTER_HANDLERS_H
#define ZERV0R_ZV_ROUTER_HANDLERS_H

#include "zv_reqStruct.h"


typedef struct {
    char *router;
    routerHandler handler;
}routers_T, pRouters_T;

void zvHandleRouters(int sock, pRequest_T req);
routerHandler zvHttpExceptionHandler(ZV_HTTP_CODE_E code);

routerHandler zvContainsUri(const char *uri_s, const char *uri_e);
#endif //ZERV0R_ZV_ROUTER_HANDLERS_H
