#include "zv_reqStruct.h"
#include "zv_common.h"

int hdrHostInit(pHdrHost_T *host)
{
    *host = (pHdrHost_T)malloc(sizeof(hdrHost_T));
    if (*host == NULL) {
        printf("alloc host failed\n");
        return -1;
    }

    (*host)->host = NULL;
    (*host)->port = NULL;

    return 0;
}

void hdrHostFree(pHdrHost_T *host)
{
    if (*host == NULL) {
        return;
    }

    S_FREE((*host)->host);
    S_FREE((*host)->port);

    S_FREE((*host));
}

int hdrHostParse(pHdrHost_T host, const char *value)
{
    if (host == NULL) {
        return -1;
    }

    SKIP_SPACE(value);
    char *s = value, *e = value;
    e = strchr(s, ':');
    if (e == NULL) {
        e = strchr(s, '\r');
        host->host = (char *)malloc(e-s);
        if (host->host == NULL) {
            hdrHostFree(&host);
            return -1;
        }
        strncpy(host->host, s, e-s);
    } else {
        host->host = (char *)malloc(e-s);
        if (host->host == NULL) {
            hdrHostFree(&host);
            return -1;
        }
        strncpy(host->host, s, e-s);

        SKIP_COLON(e);
        s = e;
        e = strchr(s, '\r');
        if (e == NULL) {
            hdrHostFree(&host);
            return -1;
        }
        
        host->port = (char *)malloc(e-s);
        if (host->port == NULL) {
            hdrHostFree(&host);
            return -1;
        }

        strncpy(host->port, s, e-s);
    }

    SKIP_NEWLINE(e);

    return e-value;
}

int hdrHostSet(pReqHead_S head, const char *value)
{
    if (head == NULL) {
        return -1;
    }

    if (hdrHostInit(&(head->host)) == -1) {
        return -1;
    }

    return hdrHostParse(head->host, value);
}