#ifndef __FFTS_STATIC_H__
#define __FFTS_STATIC_H__

#include "ffts.h"
#include "neon.h"

void ffts_static_rec_f(ffts_plan_t *p, float *data, size_t N) ;
void ffts_static_transform_f(ffts_plan_t *p, const void *in, void *out);

void ffts_static_rec_i(ffts_plan_t *p, float *data, size_t N) ;
void ffts_static_transform_i(ffts_plan_t *p, const void *in, void *out);

#endif
