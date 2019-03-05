#include "zv_resp.h"
#include "zv_resp_content_type.h"
#include "zv_req.h"

#define MAX_READ_ONCE   1024
static char *gHttpStatusDesc[ZV_HTTP_MAX];

static const char rep[] =
"HTTP/1.1 %d %s" CRLF
"Content-Length: %ld" CRLF
"Content-Type: %s" CRLF
CRLF;


void respInit()
{
    gHttpStatusDesc[ZV_HTTP_OK] = "OK";
    gHttpStatusDesc[ZV_HTTP_NOT_FOUND] = "NOT FOUND";
}

int respResponse(int sock, pResp_T resp)
{
    char respHead[MAX_RESP_HEAD_SIZE] = {0};
    snprintf(respHead, MAX_RESP_HEAD_SIZE, rep, resp->head->statusCode, resp->head->statusStr, resp->head->contentLength, resp->head->contentType);
//    printf("respHead : %s\n", respHead);

    write(sock, respHead, strlen(respHead));
    write(sock, resp->content, resp->head->contentLength);

    return 0;
}

int respText(int sock, ZV_HTTP_STATUS_E status, const char *text)
{
    respHead_T head = {0};
    head.statusCode = status;
    head.statusStr = gHttpStatusDesc[status];
    head.contentLength = strlen(text);
    head.contentType = ZV_TEXT_PLAIN;

    resp_T resp = {0};
    resp.head = &head;
    resp.content = text;

    return respResponse(sock, &resp);
}

char *readFromFile(const char *path, int *len)
{
#define MAX_READ_ONCE 1024
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror(path);
        return NULL;
    }

    *len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char *fcontent = (char *)malloc((*len)+1);
    if (fcontent == NULL) {
        perror("malloc fcontent failed\n");
        close(fd);
        return NULL;
    }
    memset(fcontent, 0, (*len)+1);

    ssize_t nbytes = 0;
    char *tmp = fcontent;
    while ((nbytes = read(fd, tmp, MAX_READ_ONCE)) > 0) {
        tmp += nbytes;
    }

    if (nbytes == -1) {
        perror("read from file failed\n");
        close(fd);
        ZV_S_FREE(fcontent);
    }

    close(fd);

    return fcontent;
}

int respHTMLFile(int sock, ZV_HTTP_STATUS_E status, const char *path)
{
    respHead_T head = {0};
    head.statusCode = status;
    head.statusStr = gHttpStatusDesc[status];
    head.contentLength = 0;
    head.contentType = ZV_TEXT_HTML;

    resp_T resp = {0};
    resp.head = &head;
    resp.content = readFromFile(path, &(head.contentLength));
    if (resp.content == NULL) {
        return -1;
    }

    respResponse(sock, &resp);

    ZV_S_FREE(resp.content);

    return 0;
}

int respHTMLText(int sock, ZV_HTTP_STATUS_E status, const char *text)
{
    respHead_T head = {0};
    head.statusCode = status;
    head.statusStr = gHttpStatusDesc[status];
    head.contentLength = strlen(text);
    head.contentType = ZV_TEXT_HTML;

    resp_T resp = {0};
    resp.head = &head;
    resp.content = text;

    respResponse(sock, &resp);

    return 0;
}
