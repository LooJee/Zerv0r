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

static void rootRouterHandler(int sock, pRequest_T req)
{
    respHTMLText(sock, ZV_HTTP_OK, http_hello_page);
}

static routers_T gRouters[] = {
        {"/", rootRouterHandler}
};

#define ROUTERS_SIZE (sizeof(gRouters)/sizeof(routers_T))

routerHandler zvContainsUri(const char *uri_s, const char *uri_e)
{
    size_t len = uri_e - uri_s + 1;

    for (int i = 0; i != ROUTERS_SIZE; ++i) {
        if (strncmp(gRouters[i].router, uri_s, len) == 0) {
            return gRouters[i].handler;
        }
    }

    return NULL;
}


void zvHttp404(int sock, pRequest_T req)
{
    respHTMLText(sock, ZV_HTTP_NOT_FOUND, ngx_http_error_404_page);
}

routerHandler zvHttpExceptionHandler(ZV_HTTP_CODE_E code)
{
    switch (code) {
        case ZV_404:
            return zvHttp404;
        default:
            return zvHttp404;
    }
}

void zvHandleRouters(int sock, pRequest_T req)
{
    for (int i = 0; i != ROUTERS_SIZE; ++i) {
        if (strcmp(gRouters[i].router, req->reqline->url) == 0) {
            return gRouters[i].handler(sock, req);
        }
    }

    respHTMLText(sock, ZV_HTTP_NOT_FOUND, ngx_http_error_404_page);
}