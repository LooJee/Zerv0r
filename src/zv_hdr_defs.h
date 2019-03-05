#ifndef _ZV_HDR_DEFS_H_
#define _ZV_HDR_DEFS_H_

#define HEAD_HOST           "Host"
#define HEAD_CONTENT_LEN    "Content-Length"
#define HEAD_CONTENT_TYPE   "Content-Type"

typedef enum {
    REQ_GET = 0,
    REQ_POST
}HTTP_METHOD_E;

typedef enum {
    HTTP09 = 0,
    HTTP10,
    HTTP11,
    HTTPMAX
}HTTP_VER_E;

#endif