#ifndef __FFTS_ND_H__
#define __FFTS_ND_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "ffts.h"

#ifdef HAVE_NEON 
	#include <arm_neon.h>
#endif

void ffts_free_nd(ffts_plan_t *p);
void ffts_transpose(uint64_t *in, uint64_t *out, int w, int h, uint64_t *buf);

void ffts_execute_nd(ffts_plan_t *p, const void *  in, void *  out); 
ffts_plan_t *ffts_init_nd(int rank, size_t *Ns, int sign); 
ffts_plan_t *ffts_init_2d(size_t N1, size_t N2, int sign); 

#endif
