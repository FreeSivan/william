#ifndef __FFTSINTERFACE_H__
#define __FFTSINTERFACE_H__

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ���ٸ���Ҷ�任��2/4����32λ�����㷨 */
/* ������x86ƽ̨���κδ���VFP��NEON��ARM */

/* Ӧ����ĿǰARMƽ̨���ٶ�����FFT�㷨
   �����fftw��˼�룬��̬����FFT����Ĵ���
   ������

/* ����ת�����̣�
   1��������Ҷ��
	  ����(ʵ��)������ΪN�㡣
	  ���(����)������Ϊ(N / 2) + 1��
   2��������Ҷ��
	  ����(����)������Ϊ(N / 2) + 1��
	  ���(ʵ��)������ΪN�㡣
   ������Ҷ����֮ǰ��Ҫ����������׼scale��
*/

#ifdef __cplusplus
extern "C"
{
#endif

typedef float ffts_complex[2];

struct _ffts_plan_t;
typedef struct _ffts_plan_t ffts_plan_t;

// ʵ��FFT����ʼ��FFT����ƻ����� sign = -1 ʱ������FFT���㣬�� sign = 1 ʱ������FFT����
ffts_plan_t *ffts_init_1d(size_t N, int sign);
ffts_plan_t *ffts_init_2d(size_t N1, size_t N2, int sign);
ffts_plan_t *ffts_init_nd(int rank, size_t *Ns, int sign);

// ����FFT����ʼ��FFT����ƻ����� sign = -1 ʱ������FFT���㣬�� sign = 1 ʱ������FFT����
ffts_plan_t *ffts_init_1d_real(size_t N, int sign);
ffts_plan_t *ffts_init_2d_real(size_t N1, size_t N2, int sign);
ffts_plan_t *ffts_init_nd_real(int rank, size_t *Ns, int sign);

// ִ��FFT�ƻ�
void ffts_execute(ffts_plan_t *, const void *input, void *output);
void ffts_free(ffts_plan_t *);

#ifdef __cplusplus
}
#endif

#endif
