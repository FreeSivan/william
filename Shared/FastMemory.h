#ifndef _FASTMEMORY_H
#define _FASTMEMORY_H

#include <stdint.h>

#ifdef WIN32
#include "inttypes.h"
#endif /* WIN32 */

/* 快速内存操作 */
class FastMemory
{
public:
	static void MemCopy(int16_t *pDst, int16_t *pSrc, uint32_t nSize);
	static void MemCopy(int32_t *pDst, int32_t *pSrc, uint32_t nSize);
	static void MemCopy(int64_t *pDst, int64_t *pSrc, uint32_t nSize);
	static void MemCopy(float *pDst, float *pSrc, uint32_t nSize);

public:
	static void MemMove(int16_t *pDst, int16_t *pSrc, uint32_t nSize);
	static void MemMove(int32_t *pDst, int32_t *pSrc, uint32_t nSize);
	static void MemMove(int64_t *pDst, int64_t *pSrc, uint32_t nSize);
	static void MemMove(float *pDst, float *pSrc, uint32_t nSize);

public:
	static void MemSet(int16_t *pAddr, int16_t nVal, uint32_t nSize);
	static void MemSet(int32_t *pAddr, int32_t nVal, uint32_t nSize);
	static void MemSet(int64_t *pAddr, int64_t nVal, uint32_t nSize);
	static void MemSet(float *pAddr, float nVal, uint32_t nSize);
};

#endif
