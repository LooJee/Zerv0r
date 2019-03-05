#ifndef _ZV_HDR_FUNC_H_
#define _ZV_HDR_FUNC_H_

#include "zv_reqStruct.h"

ZV_PARSE_STATE_E hdrReqlineSet(pParseStruct_T p);
ZV_PARSE_STATE_E hdrHostSet(pParseStruct_T p);
ZV_PARSE_STATE_E hdrNonetSet(pParseStruct_T p);

void hdrReqlineFree(pHdrReqLine_T *reqline);
void hdrHostFree(pHdrHost_T *host);

#endif