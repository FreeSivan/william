

#ifndef CPLXFFT_H
#define CPLXFFT_H

#include "BaseFFT.h"
#include "CPLXFFT_R24_F32.h"

#ifdef USE_FFTS
#include "FFTSInterface.h"
#endif /* USE_FFTS */

#ifdef USE_FFTS
#define MemoryAlloc(n) valloc(n)
#define MemoryFree(p) free(p)
#endif /* USE_FFTS */

#ifdef USE_FFTS
	#define CreateFFT(n)        ffts_init_1d_real(n, -1)
	#define CreateiFFT(n)       ffts_init_1d_real(n,  1)
	#define ExecuteFFT(p,x,y)   ffts_execute(p, x, y)
	#define DistroyFFT(p)		ffts_free(p)
#else
	#define CreateFFT(n)        new CPLXFFT_R24_F32(n)
	#define ExecuteFFT(p,x,y)   (p)->CDFT(x, y)
	#define ExecuteiFFT(p,x,y)  (p)->ICDFT(x, y)
	#define DistroyFFT(p)		delete p
#endif /* USE_FFTS */

class CPLXFFT : public BaseFFT {
public:
	CPLXFFT();
	~CPLXFFT();
	int setFFTsize(int n);
	int fft(double *src);
	int getReal(double *src);
	int getImage(double *src);
	int getABS(double *src);
	void cleanup();

private:
	int fftSize;

#ifdef USE_FFTS
	ffts_plan_t *cplxfft;
#else
	CPLXFFT_R24_F32 *cplxfft;
#endif /* USE_FFTS */

#ifdef USE_FFTS
	float __attribute__ ((aligned(32))) *x;
#else
	float *x;
#endif /* USE_FFTS */

#ifdef USE_FFTS
	ffts_complex __attribute__ ((aligned(32))) *y;
#else
	cplxdata_r32 *y;
#endif /* USE_FFTS */

};

#endif /* CPLXFFT_H */