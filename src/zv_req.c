#include <string.h>
#include <stdio.h>
#include "zv_req.h"


int zv_parseHead(const char *req, int size)
{
    char method[10] = {0};
    char url[10] = {0};

    for (int i = 0; i < size && req[i] != ' '; ++i) {
        method[i] = req[i];
    }

    if (strncmp(method, "GET", strlen("GET")) == 0) {
        return 0;
        printf("return \n");
    }
    else {
        return -1;
    }

    return 0;
}