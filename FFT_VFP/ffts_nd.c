#include "ffts_nd.h"

#ifdef HAVE_NEON
#include "neon.h"
#endif

void ffts_free_nd(ffts_plan_t *p) {

	int i;
	for(i=0;i<p->rank;i++) {
		
		ffts_plan_t *x = p->plans[i];
		int k;
		for(k=0;k<i;k++) {
			if(p->Ms[i] == p->Ms[k]) x = NULL;
		}
		
		if(x)	ffts_free(x);
	}

	free(p->Ns);
	free(p->Ms);
	free(p->plans);
	free(p->buf);
	free(p->transpose_buf);
	free(p);
}

#define TSIZE 8
#include <string.h>
void ffts_transpose(uint64_t *in, uint64_t *out, int w, int h, uint64_t *buf) {

#ifdef HAVE_NEON 
	size_t i,j,k;
	int linebytes = w*8;

	for(j=0;j<h;j+=8) {
		for(i=0;i<w;i+=8) {
			neon_transpose_to_buf(in + j*w + i, buf, w);

			uint64_t *p = out + i*h + j;
			uint64_t *pbuf = buf;
			uint64_t *ptemp;

			__asm__ __volatile__(
							 "mov %[ptemp], %[p]\n\t"
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vld1.32 {q8,q9}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q10,q11}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q12,q13}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q14,q15}, [%[pbuf], :128]!\n\t"
							 "vst1.32 {q8,q9}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q10,q11}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t" 
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vst1.32 {q12,q13}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q14,q15}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t"
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vld1.32 {q8,q9}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q10,q11}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q12,q13}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q14,q15}, [%[pbuf], :128]!\n\t"
							 "vst1.32 {q8,q9}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q10,q11}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t" 
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vst1.32 {q12,q13}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q14,q15}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t"
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vld1.32 {q8,q9}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q10,q11}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q12,q13}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q14,q15}, [%[pbuf], :128]!\n\t"
							 "vst1.32 {q8,q9}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q10,q11}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t" 
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vst1.32 {q12,q13}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q14,q15}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t"
							 "add %[p], %[p], %[w], lsl #3\n\t"
							 "vld1.32 {q8,q9}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q10,q11}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q12,q13}, [%[pbuf], :128]!\n\t"
							 "vld1.32 {q14,q15}, [%[pbuf], :128]!\n\t"
							 "vst1.32 {q8,q9}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q10,q11}, [%[ptemp], :128]!\n\t"
							 "mov %[ptemp], %[p]\n\t" 
							 "vst1.32 {q12,q13}, [%[ptemp], :128]!\n\t"
							 "vst1.32 {q14,q15}, [%[ptemp], :128]!\n\t"
							 
						: [p] "+r" (p), [pbuf] "+r" (pbuf), [ptemp] "+r" (ptemp)
						: [w] "r" (w)
						: "memory", "q8", "q9", "q10", "q11"
						);
		}
	}
#else
#ifdef HAVE_SSE
	uint64_t tmp[TSIZE*TSIZE] __attribute__((aligned(64)));
	int tx, ty;
	int x, y;
	int tw = w / TSIZE;
	int th = h / TSIZE;
	for (ty=0;ty<th;ty++) {
		for (tx=0;tx<tw;tx++) {
			uint64_t *ip0 = in + w*TSIZE*ty + tx * TSIZE;
			uint64_t *op0 = tmp;
			for (y=0;y<TSIZE;y+=2) {
					__m128d q0 = _mm_load_pd((double *)(ip0 + 0*w));
					__m128d q1 = _mm_load_pd((double *)(ip0 + 1*w));
					__m128d q2 = _mm_load_pd((double *)(ip0 + 2*w));
					__m128d q3 = _mm_load_pd((double *)(ip0 + 3*w));
					__m128d q4 = _mm_load_pd((double *)(ip0 + 4*w));
					__m128d q5 = _mm_load_pd((double *)(ip0 + 5*w));
					__m128d q6 = _mm_load_pd((double *)(ip0 + 6*w));
					__m128d q7 = _mm_load_pd((double *)(ip0 + 7*w));
					ip0 += 2;	
					
					__m128d t0 = _mm_shuffle_pd(q0, q1, _MM_SHUFFLE2(0, 0));
					__m128d t1 = _mm_shuffle_pd(q0, q1, _MM_SHUFFLE2(1, 1));
					__m128d t2 = _mm_shuffle_pd(q2, q3, _MM_SHUFFLE2(0, 0));
					__m128d t3 = _mm_shuffle_pd(q2, q3, _MM_SHUFFLE2(1, 1));
					__m128d t4 = _mm_shuffle_pd(q4, q5, _MM_SHUFFLE2(0, 0));
					__m128d t5 = _mm_shuffle_pd(q4, q5, _MM_SHUFFLE2(1, 1));
					__m128d t6 = _mm_shuffle_pd(q6, q7, _MM_SHUFFLE2(0, 0));
					__m128d t7 = _mm_shuffle_pd(q6, q7, _MM_SHUFFLE2(1, 1));
					_mm_store_pd((double *)(op0 + 0), t0);
					_mm_store_pd((double *)(op0 + 0 + TSIZE), t1);
					_mm_store_pd((double *)(op0 + 2 ), t2);
					_mm_store_pd((double *)(op0 + 2 + TSIZE), t3);
					_mm_store_pd((double *)(op0 + 4 ), t4);
					_mm_store_pd((double *)(op0 + 4 + TSIZE), t5);
					_mm_store_pd((double *)(op0 + 6 ), t6);
					_mm_store_pd((double *)(op0 + 6 + TSIZE), t7);
				op0 += 2*TSIZE;
			}
			
			op0 = out + h*tx*TSIZE + ty*TSIZE;
			ip0 = tmp;
			for (y=0;y<TSIZE;y+=1) {
				__m128d q0 = _mm_load_pd((double *)(ip0 + 0));
				__m128d q1 = _mm_load_pd((double *)(ip0 + 2));
				__m128d q2 = _mm_load_pd((double *)(ip0 + 4));
				__m128d q3 = _mm_load_pd((double *)(ip0 + 6));
				_mm_store_pd((double *)(op0 + 0), q0);
				_mm_store_pd((double *)(op0 + 2), q1);
				_mm_store_pd((double *)(op0 + 4), q2);
				_mm_store_pd((double *)(op0 + 6), q3);
				op0 += w;
				ip0 += TSIZE;
			}

		}
	}
#endif
#endif
}

void ffts_execute_nd(ffts_plan_t *p, const void *  in, void *  out) {

	uint64_t *din = (uint64_t *)in;
	uint64_t *buf = p->buf;
	uint64_t *dout = (uint64_t *)out;

	size_t i,j;
	for(i=0;i<p->Ns[0];i++) {
		p->plans[0]->transform(p->plans[0], din + (i * p->Ms[0]), buf + (i * p->Ms[0]));	
	}
	ffts_transpose(buf, dout, p->Ms[0], p->Ns[0], p->transpose_buf);	

	for(i=1;i<p->rank;i++) {
		for(j=0;j<p->Ns[i];j++) { 
			p->plans[i]->transform(p->plans[i], dout + (j * p->Ms[i]), buf + (j * p->Ms[i]));	
		}
		ffts_transpose(buf, dout, p->Ms[i], p->Ns[i], p->transpose_buf);	
	}
}

ffts_plan_t *ffts_init_nd(int rank, size_t *Ns, int sign) {
	size_t vol = 1;

	ffts_plan_t *p = malloc(sizeof(ffts_plan_t));

	p->transform = &ffts_execute_nd;
	p->destroy = &ffts_free_nd;

	p->rank = rank;
	p->Ns = malloc(sizeof(size_t) * rank);
	p->Ms = malloc(sizeof(size_t) * rank);
	p->plans = malloc(sizeof(ffts_plan_t **) * rank);
	int i;
	for(i=0;i<rank;i++) {
		p->Ns[i] = Ns[i];
		vol *= Ns[i];	
	}
	p->buf = valloc(sizeof(float) * 2 * vol);

	for(i=0;i<rank;i++) {
		p->Ms[i] = vol / p->Ns[i];

		p->plans[i] = NULL;
		int k;
		for(k=0;k<i;k++) {
			if(p->Ms[k] == p->Ms[i]) 
				p->plans[i] = p->plans[k];
		}

		if(!p->plans[i]) p->plans[i] = ffts_init_1d(p->Ms[i], sign); 
	}

	p->transpose_buf = valloc(sizeof(float) * 2 * 8 * 8);
	return p;
}

ffts_plan_t *ffts_init_2d(size_t N1, size_t N2, int sign) {
	size_t Ns[2];
	Ns[0] = N1;
	Ns[1] = N2;
	return ffts_init_nd(2, Ns, sign);
}
