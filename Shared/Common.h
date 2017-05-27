#ifndef _COMMON_H
#define _COMMON_H

#ifdef WIN32
#include "inttypes.h"
#endif /* WIN32 */

#if HAVE_NEON
#include <arm_neon.h>
#endif

/* �����Ż����������޸ģ�������Ҫ�޸Ķ�Ӧ�Ĵ��� */
/* �Ż��������CPU��
   ARM9/11, VFP, NEON, Intel Atom/Pentium4�����, AMD K9�����
*/

/* CPU��ˮ���Ż���Ĭ���Ż�4����ˮ�� */
#define CACHE_STEP 4
#define CACHE_MASK (CACHE_STEP - 1)

/* 32λ����SIMDָ���Ż� */
#define SIMD_R32_STEP 4
#define SIMD_R32_MASK (SIMD_R32_STEP - 1)

/* 16λ����SIMDָ���Ż� */
#define SIMD_I16_STEP 8
#define SIMD_I16_MASK (SIMD_I16_STEP - 1)

/* 32λ����SIMDָ���Ż� */
#define SIMD_I32_STEP 4
#define SIMD_I32_MASK (SIMD_I32_STEP - 1)

/* 64λ����SIMDָ���Ż�������32λ�����˷� */
#define SIMD_I64_STEP 2
#define SIMD_I64_MASK (SIMD_I64_STEP - 1)

#endif
