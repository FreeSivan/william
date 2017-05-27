#ifndef __NEON_H__
#define __NEON_H__

#include "ffts.h"

void neon_x4(float *, size_t, float *);
void neon_x8(float *, size_t, float *);
void neon_x8_t(float *, size_t, float *);
void neon_ee();
void neon_oo();
void neon_eo();
void neon_oe();
void neon_end();

void neon_transpose(uint64_t *in, uint64_t *out, int w, int h); 
void neon_transpose_to_buf(uint64_t *in, uint64_t *out, int w); 

void neon_static_e_f(ffts_plan_t * , const void * , void * );
void neon_static_o_f(ffts_plan_t * , const void * , void * );
void neon_static_x4_f(float *, size_t, float *);
void neon_static_x8_f(float *, size_t, float *);
void neon_static_x8_t_f(float *, size_t, float *);

void neon_static_e_i(ffts_plan_t * , const void * , void * );
void neon_static_o_i(ffts_plan_t * , const void * , void * );
void neon_static_x4_i(float *, size_t, float *);
void neon_static_x8_i(float *, size_t, float *);
void neon_static_x8_t_i(float *, size_t, float *);

#endif
