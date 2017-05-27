#ifndef __MACROS_NEON_H__
#define __MACROS_NEON_H__

#include "neon.h"
#include <arm_neon.h>

typedef float32x4_t V;
typedef float32x4x2_t VS;

#define ADD vaddq_f32
#define SUB vsubq_f32
#define MUL vmulq_f32
#define VADD vaddq_f32
#define VSUB vsubq_f32
#define VMUL vmulq_f32
#define VXOR(x,y) (vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(x), vreinterpretq_u32_f32(y))))
#define VST vst1q_f32
#define VLD vld1q_f32 
#define VST2 vst2q_f32
#define VLD2 vld2q_f32 

#define VSWAPPAIRS(x) (vrev64q_f32(x))

#define VUNPACKHI(a,b) (vcombine_f32(vget_high_f32(a), vget_high_f32(b)))
#define VUNPACKLO(a,b) (vcombine_f32(vget_low_f32(a), vget_low_f32(b)))

#define VBLEND(x,y) (vcombine_f32(vget_low_f32(x), vget_high_f32(y)))

__INLINE V VLIT4(data_t f3, data_t f2, data_t f1, data_t f0) {
    data_t __attribute__ ((aligned(16))) d[4] = {f0, f1, f2, f3};
    return VLD(d);
}

#define VDUPRE(r) vcombine_f32(vdup_lane_f32(vget_low_f32(r),0), vdup_lane_f32(vget_high_f32(r),0))
#define VDUPIM(r) vcombine_f32(vdup_lane_f32(vget_low_f32(r),1), vdup_lane_f32(vget_high_f32(r),1))

#define FFTS_MALLOC(d,a) (valloc(d))
#define FFTS_FREE(d) (free(d))

__INLINE void STORESPR(data_t * addr,  VS p) {
	vst1q_f32(addr, p.val[0]);
	vst1q_f32(addr + 4, p.val[1]);
}

__INLINE V IMULI(int inv, V a) {
	if(inv) return VSWAPPAIRS(VXOR(a, VLIT4(0.0f, -0.0f, 0.0f, -0.0f)));
	else    return VSWAPPAIRS(VXOR(a, VLIT4(-0.0f, 0.0f, -0.0f, 0.0f)));
}

__INLINE V IMUL(V d, V re, V im) {
	re = VMUL(re, d);                   
	im = VMUL(im, VSWAPPAIRS(d));
  return VSUB(re, im);  
}

__INLINE V IMULJ(V d, V re, V im) {
	re = VMUL(re, d);                   
	im = VMUL(im, VSWAPPAIRS(d));
	return VADD(re, im);  
}

#endif
