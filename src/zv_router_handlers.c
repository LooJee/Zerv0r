//
// Created by Zer0 on 2019/1/23.
//

#include "zv_router_handlers.h"
#include "zv_reqStruct.h"
#include "zv_resp.h"
#include "zv_common.h"

static const char ngx_http_error_404_page[] =
        "<html>" CRLF
        "<head><title>404 Not Found</title></head>" CRLF
        "<body>" CRLF
        "<center><h1>404 Not Found</h1></center>" CRLF
;

static const char http_hello_page[] =
        "<html>" CRLF
        "<head><title>Welcome</title></head>" CRLF
        "<body>" CRLF
        "<center><h1>Hello World!</h1></center>" CRLF
;

static void rootRouterHandler(int sock, pReqHead_S head)
{
    respHTMLText(sock, ZV_HTTP_OK, http_hello_page);
}

static routers_T gRouters[] = {
        {"/", rootRouterHandler}
};

#define ROUTERS_SIZE (sizeof(gRouters)/sizeof(routers_T))

void zvHandleRouters(int sock, pReqHead_S head)
{
    for (int i = 0; i != ROUTERS_SIZE; ++i) {
        if (strcmp(gRouters[i].router, head->reqline->url) == 0) {
            return gRouters[i].handler(sock, head);
        }
    }

    respHTMLText(sock, ZV_HTTP_NOT_FOUND, ngx_http_error_404_page);
}