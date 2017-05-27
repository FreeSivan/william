#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FastMemory.h"
#include "Common.h"

/* 快速内存操作，适用于ARM9/11, VFP, NEON和x86 */

#if HAVE_NEON

void FastMemory::MemCopy(int16_t *pDst, int16_t *pSrc, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(int16_t);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_I16_MASK);
	int32_t x = nScalar;

	do {
		vst1q_s16(pDst, vld1q_s16(pSrc));
		pSrc += SIMD_I16_STEP;
		pDst += SIMD_I16_STEP;
	} while (x -= SIMD_I16_STEP);
	
	if (nTotalCount & SIMD_I16_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pDst++ = *pSrc++;
		}
	}
}

void FastMemory::MemCopy(int32_t *pDst, int32_t *pSrc, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(int32_t);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_I32_MASK);
	int32_t x = nScalar;

	do {
		vst1q_s32(pDst, vld1q_s32(pSrc));
		pSrc += SIMD_I32_STEP;
		pDst += SIMD_I32_STEP;
	} while (x -= SIMD_I32_STEP);
	
	if (nTotalCount & SIMD_I32_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pDst++ = *pSrc++;
		}
	}
}

void FastMemory::MemCopy(int64_t *pDst, int64_t *pSrc, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(int64_t);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_I64_MASK);
	int32_t x = nScalar;

	do {
		vst1q_s64(pDst, vld1q_s64(pSrc));
		pSrc += SIMD_I64_STEP;
		pDst += SIMD_I64_STEP;
	} while (x -= SIMD_I64_STEP);
	
	if (nTotalCount & SIMD_I64_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pDst++ = *pSrc++;
		}
	}
}

void FastMemory::MemCopy(float *pDst, float *pSrc, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(float);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_R32_MASK);
	int32_t x = nScalar;

	do {
		vst1q_f32(pDst, vld1q_f32(pSrc));
		pSrc += SIMD_R32_STEP;
		pDst += SIMD_R32_STEP;
	} while (x -= SIMD_R32_STEP);
	
	if (nTotalCount & SIMD_R32_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pDst++ = *pSrc++;
		}
	}
}

void FastMemory::MemSet(int16_t *pAddr, int16_t nVal, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(int16_t);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_I16_MASK);
	int32_t x = nScalar;
	int16x8_t val = vdupq_n_s16(nVal);

	do {
		vst1q_s16(pAddr, val);
		pAddr += SIMD_I16_STEP;
	} while (x -= SIMD_I16_STEP);
	
	if (nTotalCount & SIMD_I16_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pAddr++ = nVal;
		}
	}
}

void FastMemory::MemSet(int32_t *pAddr, int32_t nVal, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(int32_t);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_I32_MASK);
	int32_t x = nScalar;
	int32x4_t val = vdupq_n_s32(nVal);

	do {
		vst1q_s32(pAddr, val);
		pAddr += SIMD_I32_STEP;
	} while (x -= SIMD_I32_STEP);
	
	if (nTotalCount & SIMD_I32_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pAddr++ = nVal;
		}
	}
}

void FastMemory::MemSet(int64_t *pAddr, int64_t nVal, uint32_t nSize) {
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(int64_t);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_I64_MASK);
	int32_t x = nScalar;
	int64x2_t val = vdupq_n_s64(nVal);

	do {
		vst1q_s64(pAddr, val);
		pAddr += SIMD_I64_STEP;
	} while (x -= SIMD_I64_STEP);
	
	if (nTotalCount & SIMD_I64_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pAddr++ = nVal;
		}
	}
}

void FastMemory::MemSet(float *pAddr, float nVal, uint32_t nSize)
{
	if (!nSize) return;

	int32_t nTotalCount = nSize / sizeof(float);
	int32_t nScalar = nTotalCount - (nTotalCount & SIMD_R32_MASK);
	int32_t x = nScalar;
	float32x4_t val = vdupq_n_f32(nVal);

	do {
		vst1q_f32(pAddr, val);
		pAddr += SIMD_R32_STEP;
	} while (x -= SIMD_R32_STEP);
	
	if (nTotalCount & SIMD_R32_MASK) {
		for (int32_t i = nScalar; i < nTotalCount; i++) {
			*pAddr++ = nVal;
		}
	}
}

#else

void FastMemory::MemCopy(int16_t *pDst, int16_t *pSrc, uint32_t nSize) {
	memcpy(pDst, pSrc, nSize);
}

void FastMemory::MemCopy(int32_t *pDst, int32_t *pSrc, uint32_t nSize) {
	memcpy(pDst, pSrc, nSize);
}

void FastMemory::MemCopy(int64_t *pDst, int64_t *pSrc, uint32_t nSize) {
	memcpy(pDst, pSrc, nSize);
}

void FastMemory::MemCopy(float *pDst, float *pSrc, uint32_t nSize) {
	memcpy(pDst, pSrc, nSize);
}

void FastMemory::MemSet(int16_t *pAddr, int16_t nVal, uint32_t nSize) {
	memset(pAddr, nVal, nSize);
}

void FastMemory::MemSet(int32_t *pAddr, int32_t nVal, uint32_t nSize) {
	memset(pAddr, nVal, nSize);
}

void FastMemory::MemSet(int64_t *pAddr, int64_t nVal, uint32_t nSize) {
	memset(pAddr, (int32_t)nVal, nSize);
}

void FastMemory::MemSet(float *pAddr, float nVal, uint32_t nSize) {
	memset(pAddr, (int32_t)nVal, nSize);
}

#endif

/* 若使用SIMD做memmove会导致异常 */

void FastMemory::MemMove(int16_t *pDst, int16_t *pSrc, uint32_t nSize) {
	memmove(pDst, pSrc, nSize);
}

void FastMemory::MemMove(int32_t *pDst, int32_t *pSrc, uint32_t nSize) {
	memmove(pDst, pSrc, nSize);
}

void FastMemory::MemMove(int64_t *pDst, int64_t *pSrc, uint32_t nSize) {
	memmove(pDst, pSrc, nSize);
}

void FastMemory::MemMove(float *pDst, float *pSrc, uint32_t nSize) {
	memmove(pDst, pSrc, nSize);
}
