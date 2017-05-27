#ifndef __CP_SSE_H__
#define __CP_SSE_H__

#include "fftsconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include "fftstypes.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209

static const __attribute__ ((aligned(64))) float w_data[16] = {0.70710678118654757273731092936941,0.70710678118654746171500846685376, -0.70710678118654757273731092936941,-0.70710678118654746171500846685376,
																								  1.0f,0.70710678118654757273731092936941f, -0.0f,-0.70710678118654746171500846685376,
																									0.70710678118654757273731092936941,0.70710678118654746171500846685376, 0.70710678118654757273731092936941,0.70710678118654746171500846685376,
																									1.0f,0.70710678118654757273731092936941f, 0.0f,0.70710678118654746171500846685376};

__INLINE float W_re(float N, float k) { return cos(-2.0f * PI * k / N); }
__INLINE float W_im(float N, float k) { return sin(-2.0f * PI * k / N); }

typedef size_t transform_index_t;

typedef void (*transform_func_t)(float *data, size_t N, float *LUT);

typedef struct _ffts_plan_t ffts_plan_t;

struct _ffts_plan_t {
	ptrdiff_t *offsets;
#ifdef DYNAMIC_DISABLED
	void *ws;
	void  *oe_ws, *eo_ws, *ee_ws;
#else
	void __attribute__((aligned(32))) *ws;
	void __attribute__((aligned(32)))  *oe_ws, *eo_ws, *ee_ws;
#endif
	ptrdiff_t *is;
	size_t *ws_is;
	size_t i0, i1, n_luts;
	size_t N;
	void *lastlut;
	transform_index_t *transforms; 
	void (*transform)(ffts_plan_t * , const void * , void * );
	void *transform_base;
	size_t transform_size;
	void *constants;

	struct _ffts_plan_t **plans;
	int rank;
	size_t *Ns, *Ms;
	void *buf;

	void *transpose_buf;

	void (*destroy)(ffts_plan_t *);

	float *A, *B;
	size_t i2;
};

void ffts_free(ffts_plan_t *);
ffts_plan_t *ffts_init_1d(size_t N, int sign); 
void ffts_execute(ffts_plan_t *, const void *, void *);

#endif
