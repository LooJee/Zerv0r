#ifndef _ZV_REQSTRUCT_H_
#define _ZV_REQSTRUCT_H_

#define MAX_METHOD_SIZE     16
#define MAX_URL_SIZE        128
#define MAX_VERSION_SIZE    16
#define MAX_HOST_SIZE       64
#define MAX_CONNECTION_SIZE 16

typedef struct {
    char *method;
    char *url;
    char *version;
}hdrReqLine_T, *pHdrReqLine_T;

typedef struct {
    char *Host;
}hdrHost_T, *pHdrHost_T;

typedef struct {
    char *connType;
}hdrConn_T, *pHdrConn_T;

typedef struct {
    pHdrReqLine_T reqline;
    char Host[MAX_HOST_SIZE];
    char Connection[MAX_CONNECTION_SIZE];
}reqHead_S, *pReqHead_S;

#endif
