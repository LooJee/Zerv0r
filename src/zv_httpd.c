#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "zv_httpd.h"
#include "zv_req.h"

const char rep[] = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\nConTent-Type: text/plain\r\n\r\nHello world";

int powerOn(void)
{
    struct sockaddr_in srv;
    int client = 0;
    int nbytes = 0;
    char buf[1024] = {0};
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int httpd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    srv.sin_port = htons(9999);
    srv.sin_family = AF_INET;

    if (setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
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
        client = accept(httpd, NULL, NULL);
        if (client == -1) {
            perror("accept new connection failed\n");
            continue;
        }
        printf("new connection...\n");
        while ((nbytes = read(client, buf, 1024)) > 0) {
            printf("%s", buf);
            if (zv_parseHead(buf, nbytes) == 0) {
                printf("hello world\n");
                write(client, rep, strlen(rep));
            }
            else {
                printf("goodbye\n");
            }
            // write(client, buf, nbytes);
            memset(buf, 0, 1024);
        }

        if (nbytes < 0) {
            perror("read failed\n");
        }
    }

    return 0;
}