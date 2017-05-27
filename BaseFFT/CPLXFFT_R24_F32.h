#ifndef _CPLXFFT_R24_F32_H
#define _CPLXFFT_R24_F32_H

#include <stdlib.h>
#include <stdint.h>

#ifdef WIN32
#include "inttypes.h"
#endif /* WIN32 */

/* 傅里叶频域数据类型
   复数类型，cplxdata_r32[0]为实部，cplxdata_r32[1]为虚部
*/
typedef float cplxdata_r32[2];

/* 快速傅里叶变换，2/4基，32位浮点算法 */
/* 可用于x86平台或Cortex A9的ARM */
/* 数据转换过程：
   1、正向傅里叶：
	  输入(实数)，长度为N点。
	  输出(复数)，长度为(N / 2) + 1点
   2、逆向傅里叶：
	  输入(复数)，长度为(N / 2) + 1点
	  输出(实数)，长度为N点。
   逆向傅里叶计算之前需要对数据做标准scale。
*/
class CPLXFFT_R24_F32 {
private:
	float *workmem;
	float *wtable;
	int32_t *iptable;
	int32_t fftlength;

private:
	void cdft(int32_t n, int32_t isgn, float *a, int32_t *ip, float *w);

private:
	void makewt(int32_t nw, int32_t *ip, float *w);

private:
	void bitrv2(int32_t n, int32_t *ip, float *a);
	void bitrv2conj(int32_t n, int32_t *ip, float *a);
	void cftfsub(int32_t n, float *a, float *w);
	void cftbsub(int32_t n, float *a, float *w);
	void cft1st(int32_t n, float *a, float *w);
	void cftmdl(int32_t n, int32_t l, float *a, float *w);

public:
	CPLXFFT_R24_F32(int32_t n);	/* n为傅里叶点数，2/4基 */
	~CPLXFFT_R24_F32();

public:
	void CDFT(float *xt, cplxdata_r32 *xf);
	void ICDFT(cplxdata_r32 *xf, float *xt);
};

#endif
