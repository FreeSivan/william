#include "ffts_static.h"

void ffts_static_rec_i(ffts_plan_t *p, float *data, size_t N) {
	if(N > 16) {
		size_t N1 = N >> 1;
		size_t N2 = N >> 2;
		size_t N3 = N >> 3;
		float *ws = ((float *)(p->ws)) + (p->ws_is[__builtin_ctzl(N)-4] << 1);

		ffts_static_rec_i(p, data, N2);
		ffts_static_rec_i(p, data + N1, N3);
		ffts_static_rec_i(p, data + N1 + N2, N3);
		ffts_static_rec_i(p, data + N, N2);
		ffts_static_rec_i(p, data + N + N1, N2);

  	if(N == p->N) {
  		neon_static_x8_t_i(data, N, ws);
  	}else{
  		neon_static_x8_i(data, N, ws); 
  	}

	}else if(N==16){
	  neon_static_x4_i(data, N, p->ws);
	}

}

void ffts_static_rec_f(ffts_plan_t *p, float *data, size_t N) {
	if(N > 16) {
		size_t N1 = N >> 1;
		size_t N2 = N >> 2;
		size_t N3 = N >> 3;
		float *ws = ((float *)(p->ws)) + (p->ws_is[__builtin_ctzl(N)-4] << 1);

		ffts_static_rec_f(p, data, N2);
		ffts_static_rec_f(p, data + N1, N3);
		ffts_static_rec_f(p, data + N1 + N2, N3);
		ffts_static_rec_f(p, data + N, N2);
		ffts_static_rec_f(p, data + N + N1, N2);

  	if(N == p->N) {
  		neon_static_x8_t_f(data, N, ws);
  	}else{
  		neon_static_x8_f(data, N, ws); 
  	}

	}else if(N==16){
	  neon_static_x4_f(data, N, p->ws);
	}

}

void ffts_static_transform_f(ffts_plan_t *p, const void *in, void *out) {

	if(__builtin_ctzl(p->N) & 1) 
		neon_static_o_f(p, in, out);
	else 
		neon_static_e_f(p, in, out);
	ffts_static_rec_f(p, out, p->N);
}

void ffts_static_transform_i(ffts_plan_t *p, const void *in, void *out) {

	if(__builtin_ctzl(p->N) & 1) 
		neon_static_o_i(p, in, out);
	else 
		neon_static_e_i(p, in, out);
	ffts_static_rec_i(p, out, p->N);
}
