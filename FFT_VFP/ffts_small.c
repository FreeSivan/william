#include "ffts.h"
#include "macros.h"
#include <stdlib.h>

#define DEBUG(x)

#include "ffts_small.h"

void firstpass_16_f(ffts_plan_t *  p, const void *  in, void *  out) {
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    V r0_1,r2_3,r4_5,r6_7,r8_9,r10_11,r12_13,r14_15;
    float *LUT8 = p->ws;

    L_4_4(0, din+0,din+16,din+8,din+24,&r0_1,&r2_3,&r8_9,&r10_11);
    L_2_4(0, din+4,din+20,din+28,din+12,&r4_5,&r6_7,&r14_15,&r12_13);
    K_N(0, VLD(LUT8),VLD(LUT8+4),&r0_1,&r2_3,&r4_5,&r6_7);
    K_N(0, VLD(LUT8+8),VLD(LUT8+12),&r0_1,&r4_5,&r8_9,&r12_13);
    S_4(r0_1,r4_5,r8_9,r12_13,dout+0,dout+8,dout+16,dout+24);
    K_N(0, VLD(LUT8+16),VLD(LUT8+20),&r2_3,&r6_7,&r10_11,&r14_15);
    S_4(r2_3,r6_7,r10_11,r14_15,dout+4,dout+12,dout+20,dout+28);
}

void firstpass_16_b(ffts_plan_t *  p, const void *  in, void *  out) {
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    V r0_1,r2_3,r4_5,r6_7,r8_9,r10_11,r12_13,r14_15;
    float *LUT8 = p->ws;

    L_4_4(1, din+0,din+16,din+8,din+24,&r0_1,&r2_3,&r8_9,&r10_11);
    L_2_4(1, din+4,din+20,din+28,din+12,&r4_5,&r6_7,&r14_15,&r12_13);
    K_N(1, VLD(LUT8),VLD(LUT8+4),&r0_1,&r2_3,&r4_5,&r6_7);
    K_N(1, VLD(LUT8+8),VLD(LUT8+12),&r0_1,&r4_5,&r8_9,&r12_13);
    S_4(r0_1,r4_5,r8_9,r12_13,dout+0,dout+8,dout+16,dout+24);
    K_N(1, VLD(LUT8+16),VLD(LUT8+20),&r2_3,&r6_7,&r10_11,&r14_15);
    S_4(r2_3,r6_7,r10_11,r14_15,dout+4,dout+12,dout+20,dout+28);
}


void firstpass_8_f(ffts_plan_t *p, const void *in, void *out)
{
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    V r0_1, r2_3, r4_5, r6_7;
    float *LUT8 = p->ws + p->ws_is[0];

    L_4_2(0, din, din+8, din+4, din+12, &r0_1, &r2_3, &r4_5, &r6_7);
    K_N(0, VLD(LUT8), VLD(LUT8+4), &r0_1, &r2_3, &r4_5, &r6_7);
    S_4(r0_1,r2_3,r4_5,r6_7,dout+0,dout+4,dout+8,dout+12);
}

void firstpass_8_b(ffts_plan_t *p, const void *in, void *out) {
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    V r0_1, r2_3, r4_5, r6_7;
    float *LUT8 = p->ws + p->ws_is[0];

    L_4_2(1, din, din+8, din+4, din+12, &r0_1, &r2_3, &r4_5, &r6_7);
    K_N(1, VLD(LUT8), VLD(LUT8+4), &r0_1, &r2_3, &r4_5, &r6_7);
    S_4(r0_1,r2_3,r4_5,r6_7,dout+0,dout+4,dout+8,dout+12);
}


void firstpass_4_f(ffts_plan_t *p, const void *in, void *out) {
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    cdata_t t0, t1, t2, t3, t4, t5, t6, t7;
    t0[0] = din[0]; t0[1] = din[1];
    t1[0] = din[4]; t1[1] = din[5];
    t2[0] = din[2]; t2[1] = din[3];
    t3[0] = din[6]; t3[1] = din[7];
        
    t4[0] = t0[0] + t1[0]; t4[1] = t0[1] + t1[1];
    t5[0] = t0[0] - t1[0]; t5[1] = t0[1] - t1[1];
    t6[0] = t2[0] + t3[0]; t6[1] = t2[1] + t3[1];
    t7[0] = t2[0] - t3[0]; t7[1] = t2[1] - t3[1];

    dout[0] = t4[0] + t6[0]; dout[1] = t4[1] + t6[1];
    dout[4] = t4[0] - t6[0]; dout[5] = t4[1] - t6[1];
    dout[2] = t5[0] + t7[1]; dout[3] = t5[1] - t7[0];
    dout[6] = t5[0] - t7[1]; dout[7] = t5[1] + t7[0];
}

void firstpass_4_b(ffts_plan_t *p, const void *in, void *out) {
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    cdata_t t0, t1, t2, t3, t4, t5, t6, t7;
    t0[0] = din[0]; t0[1] = din[1];
    t1[0] = din[4]; t1[1] = din[5];
    t2[0] = din[2]; t2[1] = din[3];
    t3[0] = din[6]; t3[1] = din[7];
        
    t4[0] = t0[0] + t1[0]; t4[1] = t0[1] + t1[1];
    t5[0] = t0[0] - t1[0]; t5[1] = t0[1] - t1[1];
    t6[0] = t2[0] + t3[0]; t6[1] = t2[1] + t3[1];
    t7[0] = t2[0] - t3[0]; t7[1] = t2[1] - t3[1];

    dout[0] = t4[0] + t6[0]; dout[1] = t4[1] + t6[1];
    dout[4] = t4[0] - t6[0]; dout[5] = t4[1] - t6[1];
    dout[2] = t5[0] - t7[1]; dout[3] = t5[1] + t7[0];
    dout[6] = t5[0] + t7[1]; dout[7] = t5[1] - t7[0];
}

void firstpass_2(ffts_plan_t *p, const void *in, void *out) {
    const data_t *din = (const data_t *)in;
    data_t *dout = (data_t *)out;
    cdata_t t0, t1, r0,r1;
    t0[0] = din[0]; t0[1] = din[1];
    t1[0] = din[2]; t1[1] = din[3];
    r0[0] = t0[0] + t1[0];
    r0[1] = t0[1] + t1[1];
    r1[0] = t0[0] - t1[0];
    r1[1] = t0[1] - t1[1];
    dout[0] = r0[0]; dout[1] = r0[1];
    dout[2] = r1[0]; dout[3] = r1[1];
}
