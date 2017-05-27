/*
 * create by sivan.yue
 * data : 2017-05-27
 */


#ifndef CAFP_H
#define CAFP_H

#include "../BaseFFT/fftCommon.h"
#include "../BaseWindow/wCommon.h"

#define NUM_BANDS 33
#define NUM_FREQS 34

class CAFP {
public:
	CAFP();
	~CAFP();

	int setParm(double nfs, double stepTime,double dataTime, int nfftSize, int mode);
	int getSubFingerprinting(short *src, int srcSize,void *fingerbuf, int maxSize);
	void cleanup();
	void newStart();

	int calcuteNextSubFingerSize(int len);

private:
	void prepareData(short *src, int len);

private:
	BaseFFT *pfft;
	BaseWindow *pwindow;

	int dataSize, fftSize, hopSize;
	double fs;
	double *freqs;
	double *lastData, *curData;
	double *fftbuf;
	int lastSize, curSize; // last pcm size(short type), last + new pcm size(short type)
	int maxLastSize, maxCurSize;

	int xovr[NUM_FREQS];
	double be[NUM_BANDS * 2];
	double *be_old;
};

#endif /* CAFP_H */
