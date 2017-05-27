
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "CPLXFFT.h"

using namespace std;

CPLXFFT::CPLXFFT():
	fftSize(4096),
	cplxfft(NULL), x(NULL),
	y(NULL) {
}

CPLXFFT::~CPLXFFT() {
	cleanup();
}

int CPLXFFT::setFFTsize(int n) {
	if(n <= 0) {
		puts("fft size error !");
		return 1;
	}

	fftSize = n;

#ifdef USE_FFTS
	cplxfft = CreateFFT(fftSize);
#else
	cplxfft = CreateFFT(fftSize);
#endif /* USE_FFTS */

#ifdef USE_FFTS
	x = (float *)MemoryAlloc(sizeof(float) * fftSize);
#else
	x = new float[fftSize];
#endif /* USE_FFTS */

#ifdef USE_FFTS
	int dwSize = sizeof(ffts_complex) * (fftSize);
	y = (ffts_complex *)MemoryAlloc(dwSize);
#else
	y = new cplxdata_r32[fftSize];
#endif /* USE_FFTS */

	return 0;
}

int CPLXFFT::fft(double *src) {
	if(src == NULL) {
		puts("src fft data error !");
		return 1;
	}

	int i;
	for(i = 0; i < fftSize; ++i) {
		x[i] = (float)src[i];
	}

	ExecuteFFT(cplxfft, x, y);

	for(i = 1; i < fftSize / 2; ++i) {
		y[fftSize - i][0] = y[i][0];
		y[fftSize - i][1] = y[i][1];
		y[i][1] = -y[i][1];
	}
    
    return 0;
}

int CPLXFFT::getReal(double *src) {
	if(src == NULL) {
		return 1;
	}

	int i;
	for(i = 0; i < fftSize; ++i) {
		src[i] = y[i][0];
	}

	return 0;
	
}

int CPLXFFT::getImage(double *src) {
	if(src == NULL) {
		return 1;
	}

	int i;
	for(i = 0; i < fftSize; ++i) {
		src[i] = y[i][1];
	}

	return 0;
}

int CPLXFFT::getABS(double *src) {
	if(src == NULL) {
		return 1;
	}

	int i;
	for(i = 0; i < fftSize; ++i) {
		src[i] = sqrt(y[i][0] * y[i][0] + y[i][1] * y[i][1]);
	}

	return 0;
}

void CPLXFFT::cleanup() {

	if(cplxfft != NULL) {
		DistroyFFT(cplxfft);
	}
	cplxfft = NULL;


	if(x != NULL) {
#ifdef USE_FFTS
		MemoryFree(x);
#else
		delete [] x;
#endif /* USE_FFTS */
	}
	x = NULL;
	

	if(y != NULL) {
#ifdef USE_FFTS
		MemoryFree(y);
#else
		delete [] y;
#endif /* USE_FFTS */
	}
	y = NULL;

}