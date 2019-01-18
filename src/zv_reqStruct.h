#ifndef _ZV_REQSTRUCT_H_
#define _ZV_REQSTRUCT_H_

typedef struct {
    char *method;
    char *url;
    char *version;
}hdrReqLine_T, *pHdrReqLine_T;

typedef struct {
    char *host;
    char *port;
}hdrHost_T, *pHdrHost_T;

typedef struct {
    char *connType;
}hdrConn_T, *pHdrConn_T;

typedef struct {
    pHdrReqLine_T   reqline;
    pHdrHost_T      host;
}reqHead_S, *pReqHead_S;

#endif
