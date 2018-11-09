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

const char rep[] = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\nConTent-Type: text/plain\r\n\r\nHello world";

typedef struct{
    int clientfd;
} THREAD_PARAM_S;

void *handleClient(void *arg)
{
    char buf[1024] = {0};
    int ret = -1;
    int nbytes = 0;
    THREAD_PARAM_S *param = (THREAD_PARAM_S*)arg;

    printf("handle client %u\n", pthread_self());

    while ((nbytes = read(param->clientfd, buf, 1024)) >= 0) {
        if (nbytes == 0) {
            printf("read end\n");
            break;
        }
        printf("%s", buf);
        if (zv_parseHead(buf, nbytes) == 0){
            write(param->clientfd, rep, strlen(rep));
        }
        else {
            printf("goodbye\n");
        }
        memset(buf, 0, 1024);
    }

    if (nbytes < 0) {
        perror("read failed\n");
    }

    close(param->clientfd);
    free(param);
    param = NULL;

    pthread_exit((void *)&ret);
}

int powerOn(void)
{
    struct sockaddr_in srv;
    int client = 0;
    int sockOpt = 1;
    int httpd = 0;

    socklen_t addrlen = sizeof(struct sockaddr_in);
    httpd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    srv.sin_port = htons(9999);
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

        pthread_t thid;
        void *ret;
        if (pthread_create(&thid, NULL, handleClient, (void*)param)) {
            perror("create thread failed");
            continue;
        }
        printf("child thread id : %u\n", thid);

        if (!pthread_detach(thid)) {
            perror("detach thread failed");
            continue;
        }
    }

    return 0;
}
