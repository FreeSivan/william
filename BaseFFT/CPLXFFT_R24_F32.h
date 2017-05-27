#ifndef _CPLXFFT_R24_F32_H
#define _CPLXFFT_R24_F32_H

#include <stdlib.h>
#include <stdint.h>

#ifdef WIN32
#include "inttypes.h"
#endif /* WIN32 */

/* ����ҶƵ����������
   �������ͣ�cplxdata_r32[0]Ϊʵ����cplxdata_r32[1]Ϊ�鲿
*/
typedef float cplxdata_r32[2];

/* ���ٸ���Ҷ�任��2/4����32λ�����㷨 */
/* ������x86ƽ̨��Cortex A9��ARM */
/* ����ת�����̣�
   1��������Ҷ��
	  ����(ʵ��)������ΪN�㡣
	  ���(����)������Ϊ(N / 2) + 1��
   2��������Ҷ��
	  ����(����)������Ϊ(N / 2) + 1��
	  ���(ʵ��)������ΪN�㡣
   ������Ҷ����֮ǰ��Ҫ����������׼scale��
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
	CPLXFFT_R24_F32(int32_t n);	/* nΪ����Ҷ������2/4�� */
	~CPLXFFT_R24_F32();

public:
	void CDFT(float *xt, cplxdata_r32 *xf);
	void ICDFT(cplxdata_r32 *xf, float *xt);
};

#endif
