#include "ffts_real_nd.h"

#ifdef __ARM_NEON__
#include "neon.h"
#endif

void ffts_scalar_transpose(uint64_t *in, uint64_t *out, int w, int h, uint64_t *buf) {

	size_t i,j;
	for(i=0;i<w;i+=1) {
		for(j=0;j<h;j+=1) {
			out[i*h + j] = in[j*w + i];
		}
	}

}

void ffts_execute_nd_real(ffts_plan_t *p, const void *  in, void *  out) {

	uint32_t *din = (uint32_t *)in;
	uint64_t *buf = p->buf;
	uint64_t *dout = (uint64_t *)out;

	size_t i,j;
	for(i=0;i<p->Ns[0];i++) {
		p->plans[0]->transform(p->plans[0], din + (i * p->Ms[0]), buf + (i * p->Ms[0]));	
	}
	ffts_scalar_transpose(buf, dout, p->Ms[0], p->Ns[0], p->transpose_buf);	

	for(i=1;i<p->rank;i++) {
		for(j=0;j<p->Ns[i];j++) { 
			p->plans[i]->transform(p->plans[i], dout + (j * p->Ms[i]), buf + (j * p->Ms[i]));	
		}
		ffts_scalar_transpose(buf, dout, p->Ms[i], p->Ns[i], p->transpose_buf);	
	}
}

void ffts_execute_nd_real_inv(ffts_plan_t *p, const void *  in, void *  out) {

	uint64_t *din = (uint64_t *)in;
	uint64_t *buf = p->buf;
	uint64_t *dout = (uint64_t *)out;
	
	float *bufr = (float *)(p->buf);
	float *doutr = (float *)out;

	size_t i,j;
	ffts_scalar_transpose(din, buf, p->Ms[0], p->Ns[0], p->transpose_buf);	

	for(i=0;i<p->Ms[0];i++) {
		p->plans[0]->transform(p->plans[0], buf + (i * p->Ns[0]), dout + (i * p->Ns[0]));	
	}
	
	ffts_scalar_transpose(dout, buf, p->Ns[0], p->Ms[0], p->transpose_buf);	
	for(j=0;j<p->Ms[1];j++) { 
  	p->plans[1]->transform(p->plans[1], buf + (j * (p->Ms[0])), &doutr[j * p->Ns[1]]);	
  }
}

ffts_plan_t *ffts_init_nd_real(int rank, size_t *Ns, int sign) {
	size_t vol = 1;

	ffts_plan_t *p = malloc(sizeof(ffts_plan_t));

	if(sign < 0) p->transform = &ffts_execute_nd_real;
	else         p->transform = &ffts_execute_nd_real_inv;

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

		if(sign < 0) {
			for(k=1;k<i;k++) {
				if(p->Ms[k] == p->Ms[i]) p->plans[i] = p->plans[k];
			}
			if(!i)                p->plans[i] = ffts_init_1d_real(p->Ms[i], sign); 
			else if(!p->plans[i]) p->plans[i] = ffts_init_1d(p->Ms[i], sign); 
		}else{
  		for(k=0;k<i;k++) {
  			if(p->Ns[k] == p->Ns[i]) p->plans[i] = p->plans[k];
  		}
			if(i==rank-1)         p->plans[i] = ffts_init_1d_real(p->Ns[i], sign); 
			else if(!p->plans[i]) p->plans[i] = ffts_init_1d(p->Ns[i], sign); 
		}
	}
	if(sign < 0) {
		for(i=1;i<rank;i++) {
			p->Ns[i] = p->Ns[i] / 2 + 1;
		}
	}else{
  	for(i=0;i<rank-1;i++) {
  		p->Ms[i] = p->Ms[i] / 2 + 1;
  	}
	}

	p->transpose_buf = valloc(sizeof(float) * 2 * 8 * 8);
	return p;
}

ffts_plan_t *ffts_init_2d_real(size_t N1, size_t N2, int sign) {
	size_t Ns[2];
	Ns[0] = N1;
	Ns[1] = N2;
	return ffts_init_nd_real(2, Ns, sign);
}
