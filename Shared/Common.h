#ifndef _COMMON_H
#define _COMMON_H

#ifdef WIN32
#include "inttypes.h"
#endif /* WIN32 */

#if HAVE_NEON
#include <arm_neon.h>
#endif

/* 以下优化常量请勿修改，否则需要修改对应的代码 */
/* 优化针对如下CPU：
   ARM9/11, VFP, NEON, Intel Atom/Pentium4或更高, AMD K9或更高
*/

/* CPU流水线优化，默认优化4级流水线 */
#define CACHE_STEP 4
#define CACHE_MASK (CACHE_STEP - 1)

/* 32位浮点SIMD指令优化 */
#define SIMD_R32_STEP 4
#define SIMD_R32_MASK (SIMD_R32_STEP - 1)

/* 16位整数SIMD指令优化 */
#define SIMD_I16_STEP 8
#define SIMD_I16_MASK (SIMD_I16_STEP - 1)

/* 32位整数SIMD指令优化 */
#define SIMD_I32_STEP 4
#define SIMD_I32_MASK (SIMD_I32_STEP - 1)

/* 64位整数SIMD指令优化，用于32位整数乘法 */
#define SIMD_I64_STEP 2
#define SIMD_I64_MASK (SIMD_I64_STEP - 1)

#endif
