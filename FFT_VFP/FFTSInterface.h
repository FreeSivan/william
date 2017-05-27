#ifndef __FFTSINTERFACE_H__
#define __FFTSINTERFACE_H__

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* 快速傅里叶变换，2/4基，32位浮点算法 */
/* 可用于x86平台或任何带有VFP或NEON的ARM */

/* 应该是目前ARM平台上速度最快的FFT算法
   借鉴了fftw的思想，动态生成FFT计算的代码
   当定义

/* 数据转换过程：
   1、正向傅里叶：
	  输入(实数)，长度为N点。
	  输出(复数)，长度为(N / 2) + 1点
   2、逆向傅里叶：
	  输入(复数)，长度为(N / 2) + 1点
	  输出(实数)，长度为N点。
   逆向傅里叶计算之前需要对数据做标准scale。
*/

#ifdef __cplusplus
extern "C"
{
#endif

typedef float ffts_complex[2];

struct _ffts_plan_t;
typedef struct _ffts_plan_t ffts_plan_t;

// 实数FFT，初始化FFT计算计划，当 sign = -1 时做逆向FFT运算，当 sign = 1 时做正向FFT运算
ffts_plan_t *ffts_init_1d(size_t N, int sign);
ffts_plan_t *ffts_init_2d(size_t N1, size_t N2, int sign);
ffts_plan_t *ffts_init_nd(int rank, size_t *Ns, int sign);

// 复数FFT，初始化FFT计算计划，当 sign = -1 时做逆向FFT运算，当 sign = 1 时做正向FFT运算
ffts_plan_t *ffts_init_1d_real(size_t N, int sign);
ffts_plan_t *ffts_init_2d_real(size_t N1, size_t N2, int sign);
ffts_plan_t *ffts_init_nd_real(int rank, size_t *Ns, int sign);

// 执行FFT计划
void ffts_execute(ffts_plan_t *, const void *input, void *output);
void ffts_free(ffts_plan_t *);

#ifdef __cplusplus
}
#endif

#endif
