#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#include "ffts.h"

void ffts_init_is(ffts_plan_t *p, int N, int leafN, int VL);
void ffts_init_offsets(ffts_plan_t *p, int N, int leafN);

#endif
