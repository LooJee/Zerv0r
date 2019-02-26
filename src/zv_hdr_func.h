#ifndef _ZV_HDR_FUNC_H_
#define _ZV_HDR_FUNC_H_

#include "zv_reqStruct.h"

int hdrReqlineSet(pReqHead_S head, const char *value);
int hdrHostSet(pReqHead_S head, const char *value);

void hdrReqlineFree(pHdrReqLine_T *reqline);
void hdrHostFree(pHdrHost_T *host);

#endif