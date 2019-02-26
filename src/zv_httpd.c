#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "zv_httpd.h"
#include "zv_req.h"
#include "zv_reqStruct.h"
#include "zv_resp.h"
#include "zv_router_handlers.h"
#include "zv_hdr_func.h"
#include "../3rdparty/cThreadPool/src/threadpool.h"

#define BASE_DATASIZE_FEET 256

typedef struct{
    int clientfd;
} THREAD_PARAM_S;

void zvHttpFree(pReqHead_S head)
{
    if (head->reqline != NULL) {
        hdrReqlineFree(&head->reqline);
    }

    if (head->host != NULL) {
        hdrHostFree(&head->host);
    }
}

int handleRead(char *data, THREAD_PARAM_S *param)
{
    int nbytes = 0;
    int dataSize = BASE_DATASIZE_FEET;
    int offset = 0;
    int ret = 0;
    while ((nbytes = read(param->clientfd, data+offset, BASE_DATASIZE_FEET)) >= 0) {
        if (nbytes == 0) {
            break;
        } else if (nbytes < BASE_DATASIZE_FEET) {
            reqHead_S head = {0};
            if (zv_parseHead(data, dataSize, &head) == 0){
                zvHandleRouters(param->clientfd, &head);
            }
            else {
                printf("goodbye\n");
            }
            zvHttpFree(&head);
            break;
        } else {
            offset += nbytes;
            if (dataSize <= offset) {
                dataSize += BASE_DATASIZE_FEET;
                data = (char *)realloc(data, dataSize);
            }
        }
    }

    if (nbytes < 0) {
        perror("read failed\n");
        ret = -1;
    }

    S_FREE(data);

    return ret;
}

void handleClient(void *arg)
{
    char *data = NULL;
    int ret = -1;
    THREAD_PARAM_S *param = (THREAD_PARAM_S*)arg;

    data = (char *)malloc(BASE_DATASIZE_FEET);
    if (data == NULL) {
        perror("malloc data size failed");
    } else {
        handleRead(data, param);
    }

    close(param->clientfd);
    S_FREE(param);
//    pthread_exit((void *)&ret);
}

int powerOn(void)
{
    struct sockaddr_in srv;
    int client = 0;
    int sockOpt = 1;
    int httpd = 0;
    pThreadPool_T tpool = tpNew(4);

    socklen_t addrlen = sizeof(struct sockaddr_in);
    httpd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    srv.sin_port = htons(8800);
    srv.sin_family = AF_INET;

    if (setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(int)) < 0){
        perror("setsocketopt failed\n");
        return -1;
    }

    printf("power on...\n");

    if (-1 == bind(httpd, (struct sockaddr *)&srv, addrlen)) {
        perror("bind failed\n");
        return -1;
    }

    printf("bind...\n");

    if (-1 == listen(httpd, 5)) {
        perror("listen failed\n");
        return -1;
    }

    printf("listen...\n");

    while (true) {
        THREAD_PARAM_S *param = (THREAD_PARAM_S *)malloc(sizeof(THREAD_PARAM_S));

        if (param == NULL) {
            perror("malloc failed");
            exit(-1);
        }

        memset(param, 0, sizeof(THREAD_PARAM_S));

        param->clientfd = accept(httpd, NULL, NULL);
        if (param->clientfd == -1) {
            perror("accept new connection failed\n");
            continue;
        }

        pTPTask_T task = taskNew(handleClient, (void*)param, NULL, NULL);
        tpAddTask(tpool, task);
    }

    tpFree(tpool);

    return 0;
}
