#ifndef __FFTS_REAL_H__
#define __FFTS_REAL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "ffts.h"

#ifdef HAVE_NEON 
	#include <arm_neon.h>
#endif

ffts_plan_t *ffts_init_1d_real(size_t N, int sign);

#endif
