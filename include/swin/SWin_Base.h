#ifndef SWin_Base_h
#define SWin_Base_h

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define ALLOC(type, n) (type*)malloc(sizeof(type) * (n))
#define ALLOC_S(type) ALLOC(type, 1)
#define CHECK(result, msg, n) if((result) == NULL) { printf("SWin: "); printf(msg); printf("\n"); return n; }
#define CHECK_F(func, msg, n) CHECK(func, msg, n)
#define DEALLOC(ptr) free(ptr)

typedef uint8_t SBool;
typedef int8_t SResult;

#define SWIN_FALSE ((SBool)0)
#define SWIN_TRUE ((SBool)1)

#define SWIN_OK ((SResult)0)
#define SWIN_FAILED ((SResult)-1)


#endif