#include "ffts_real.h"

void ffts_free_1d_real(ffts_plan_t *p) {
	ffts_free(p->plans[0]);
	free(p->A);
	free(p->B);
	free(p->plans);
	free(p->buf);
	free(p);
}

void ffts_execute_1d_real(ffts_plan_t *p, const void *vin, void *vout) {
	float *out = (float *)vout;
	float *buf = (float *)p->buf;
	float *A = p->A;
	float *B = p->B;

	p->plans[0]->transform(p->plans[0], vin, buf);

	size_t N = p->N;
	buf[N] = buf[0];
	buf[N+1] = buf[1];

	float *p_buf0 = buf;
	float *p_buf1 = buf + N - 2;
	float *p_out = out;

	size_t i;
#ifdef __ARM_NEON__
	for(i=0;i<N/2;i+=2) {
	__asm__ __volatile__ ("vld1.32 {q8},  [%[pa], :128]!\n\t"
												"vld1.32 {q9},  [%[pb], :128]!\n\t"
												"vld1.32 {q10}, [%[buf0], :128]!\n\t"
												"vld1.32 {q11}, [%[buf1], :64]\n\t"
												"sub %[buf1], %[buf1], #16\n\t"

												"vdup.32 d26, d16[1]\n\t"
												"vdup.32 d27, d17[1]\n\t"
												"vdup.32 d24, d16[0]\n\t"
												"vdup.32 d25, d17[0]\n\t"
												
												"vdup.32 d30, d23[1]\n\t"
												"vdup.32 d31, d22[1]\n\t"
												"vdup.32 d28, d23[0]\n\t"
												"vdup.32 d29, d22[0]\n\t"
												
												"vmul.f32 q13, q13, q10\n\t"
												"vmul.f32 q15, q15, q9\n\t"
												"vmul.f32 q12, q12, q10\n\t"
												"vmul.f32 q14, q14, q9\n\t"
												"vrev64.f32 q13, q13\n\t"
												"vrev64.f32 q15, q15\n\t"

												"vtrn.32 d26, d27\n\t"
												"vtrn.32 d30, d31\n\t"
												"vneg.f32 d26, d26\n\t"
												"vneg.f32 d31, d31\n\t"
												"vtrn.32 d26, d27\n\t"
												"vtrn.32 d30, d31\n\t"
												
												"vadd.f32 q12, q12, q14\n\t"
												"vadd.f32 q13, q13, q15\n\t"
												"vadd.f32 q12, q12, q13\n\t"
												"vst1.32 {q12}, [%[pout], :128]!\n\t"
												: [pa] "+r" (A), [pb] "+r" (B), [buf0] "+r" (p_buf0), [buf1] "+r" (p_buf1),
													[pout] "+r" (p_out)
												:
												: "memory", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
												);
#else
	for(i=0;i<N/2;i++) {
		out[2*i]   = buf[2*i]*A[2*i] - buf[2*i+1]*A[2*i+1] + buf[N-2*i]*B[2*i] + buf[N-2*i+1]*B[2*i+1];
		out[2*i+1] = buf[2*i+1]*A[2*i] + buf[2*i]*A[2*i+1] + buf[N-2*i]*B[2*i+1] - buf[N-2*i+1]*B[2*i];
#endif	
	}

	out[N] = buf[0] - buf[1];
	out[N+1] = 0.0f;
}

void ffts_execute_1d_real_inv(ffts_plan_t *p, const void *vin, void *vout) {
	float *out = (float *)vout;
	float *in = (float *)vin;
	float *buf = (float *)p->buf;
	float *A = p->A;
	float *B = p->B;
	size_t N = p->N;
	
	float *p_buf0 = in;
	float *p_buf1 = in + N - 2;
	
	float *p_out = buf;
	
	size_t i;
#ifdef __ARM_NEON__
	for(i=0;i<N/2;i+=2) {
	__asm__ __volatile__ ("vld1.32 {q8},  [%[pa], :128]!\n\t"
												"vld1.32 {q9},  [%[pb], :128]!\n\t"
												"vld1.32 {q10}, [%[buf0], :128]!\n\t"
												"vld1.32 {q11}, [%[buf1], :64]\n\t"
												"sub %[buf1], %[buf1], #16\n\t"

												"vdup.32 d26, d16[1]\n\t"
												"vdup.32 d27, d17[1]\n\t"
												"vdup.32 d24, d16[0]\n\t"
												"vdup.32 d25, d17[0]\n\t"
												
												"vdup.32 d30, d23[1]\n\t"
												"vdup.32 d31, d22[1]\n\t"
												"vdup.32 d28, d23[0]\n\t"
												"vdup.32 d29, d22[0]\n\t"
												
												"vmul.f32 q13, q13, q10\n\t"
												"vmul.f32 q15, q15, q9\n\t"
												"vmul.f32 q12, q12, q10\n\t"
												"vmul.f32 q14, q14, q9\n\t"
												"vrev64.f32 q13, q13\n\t"
												"vrev64.f32 q15, q15\n\t"

												"vtrn.32 d26, d27\n\t"
												"vtrn.32 d28, d29\n\t"
												"vneg.f32 d27, d27\n\t"
												"vneg.f32 d29, d29\n\t"
												"vtrn.32 d26, d27\n\t"
												"vtrn.32 d28, d29\n\t"
												
												"vadd.f32 q12, q12, q14\n\t"
												"vsub.f32 q13, q13, q15\n\t"
												"vadd.f32 q12, q12, q13\n\t"
												"vst1.32 {q12}, [%[pout], :128]!\n\t"
												: [pa] "+r" (A), [pb] "+r" (B), [buf0] "+r" (p_buf0), [buf1] "+r" (p_buf1),
													[pout] "+r" (p_out)
												:
												: "memory", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
												);
#else
	for(i=0;i<N/2;i++) {
		buf[2*i]   = in[2*i]*A[2*i] + in[2*i+1]*A[2*i+1] + in[N-2*i]*B[2*i] - in[N-2*i+1]*B[2*i+1];
		buf[2*i+1] = in[2*i+1]*A[2*i] - in[2*i]*A[2*i+1] - in[N-2*i]*B[2*i+1] - in[N-2*i+1]*B[2*i];
#endif
}

	p->plans[0]->transform(p->plans[0], buf, out);
}

ffts_plan_t *ffts_init_1d_real(size_t N, int sign) {
	ffts_plan_t *p = malloc(sizeof(ffts_plan_t));

	if(sign < 0) p->transform = &ffts_execute_1d_real;
	else         p->transform = &ffts_execute_1d_real_inv;
	
	p->destroy = &ffts_free_1d_real;
	p->N = N;
	p->rank = 1;
	p->plans = malloc(sizeof(ffts_plan_t **) * 1);

	p->plans[0] = ffts_init_1d(N/2, sign); 

	p->buf = valloc(sizeof(float) * 2 * ((N/2) + 1));

	p->A = valloc(sizeof(float) * N);
	p->B = valloc(sizeof(float) * N);

  if(sign < 0) {
		int i;
		for (i = 0; i < N/2; i++) {
			p->A[2 * i]     = 0.5 * (1.0 - sin (2.0f * PI / (double) (N) * (double) i));
			p->A[2 * i + 1] = 0.5 * (-1.0 * cos (2.0f * PI / (double) (N) * (double) i));
			p->B[2 * i]     = 0.5 * (1.0 + sin (2.0f * PI / (double) (N) * (double) i));
			p->B[2 * i + 1] = 0.5 * (1.0 * cos (2.0f * PI / (double) (N) * (double) i));
		}
	}else{
		int i;
		for (i = 0; i < N/2; i++) {
			p->A[2 * i]     = 1.0 * (1.0 - sin (2.0f * PI / (double) (N) * (double) i));
			p->A[2 * i + 1] = 1.0 * (-1.0 * cos (2.0f * PI / (double) (N) * (double) i));
			p->B[2 * i]     = 1.0 * (1.0 + sin (2.0f * PI / (double) (N) * (double) i));
			p->B[2 * i + 1] = 1.0 * (1.0 * cos (2.0f * PI / (double) (N) * (double) i));
		}
  }
	
	return p;
}
