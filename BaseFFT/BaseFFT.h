/*
 * create by sivan.yue
 * data : 2017-05-27
 */

#ifndef BASE_FFT
#define BASE_FFT

class BaseFFT
{
public:
	BaseFFT(){}
	virtual ~BaseFFT(){};

	virtual int setFFTsize(int n){ return 0;}
	virtual int fft(double *src){ return 0;}
	virtual int getReal(double *src){ return 0;}
	virtual int getImage(double *src){ return 0;}
	virtual int getABS(double *src){ return 0;}

	virtual void cleanup(){ }
};

#endif /* BASE_FFT */