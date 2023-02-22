#ifndef _BS_H_
#define _BS_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include"global.h"
void Bs_GetbufFromJson(char *filename ,bsstruct * buf);
char * Bs_GetCharBufFromJsonFIle(char *filename);
#ifdef __cplusplus
}
#endif
#endif