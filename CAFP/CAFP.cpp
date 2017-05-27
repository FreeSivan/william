/*
 * create by sivan.yue
 * data : 2017-05-27
 */


#include <cstdlib>
#include <cstdio>
#include <string.h>

#include "CAFP.h"

//#define CAFP_DEBUG 1

#ifdef AFP_DEBUG_ANDROID
#include <android/log.h>
#define  LOG_TAG    "AFP_TEST"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif /* AFP_DEBUG_ANDOIRD */

#define MAX_SHORT 32767.0
#define MIN_SHORT 32768.0

#define s162double(s) (double)( (s) > 0 ? ((s) / (MAX_SHORT)) : ((s) / (MIN_SHORT)) )
#define double2s16(f) (short)( (f) > 0 ? ((f) * (MAX_SHORT)) : ((f) * (MIN_SHORT)) )

static double line_freqs[] = {300.00, 351.52, 403.03,
							  454.55, 506.06, 557.58,
							  609.09, 660.61, 712.12,
							  763.64, 815.15, 866.67,
							  918.18, 969.70, 1021.21,
							  1072.73, 1124.24, 1175.76,
							  1227.27, 1278.79, 1330.30,
							  1381.82, 1433.33, 1484.85,
							  1536.36, 1587.88, 1639.39,
							  1690.91, 1742.42, 1793.94,
							  1845.45, 1896.97, 1948.48, 2000.00};

static double log_freqs[] = {300.00, 317.75, 336.55,
							 356.47, 377.56, 399.90,
							 423.57, 448.63, 475.18,
							 503.30, 533.08, 564.62,
							 598.03, 633.42, 670.90,
							 710.60, 752.65, 797.18,
							 844.36, 894.32, 947.24,
							 1003.29, 1062.66, 1125.54,
							 1192.14, 1262.68, 1337.40,
							 1416.54, 1500.36, 1589.14,
							 1683.17, 1782.77, 1888.27, 2000.00};

static double bark_freqs[] = {204, 234, 265, 297, 330, 363,
							  399, 434, 471, 509, 548, 588,
							  631, 674, 718, 766, 813, 862,
							  915, 967, 1022, 1082, 1141, 1202,
							  1268, 1334, 1404, 1479, 1554,
							  1633, 1719, 1806, 1899, 1996};

CAFP::CAFP():
	pfft(NULL), pwindow(NULL),
	dataSize(0), fftSize(0), hopSize(0),
	fs(5000.0), freqs(NULL), lastData(NULL),
	curData(NULL), lastSize(0), curSize(0),
	fftbuf(NULL), maxLastSize(0), maxCurSize(0) {
	be_old = be + NUM_BANDS;
}

CAFP::~CAFP() {
	cleanup();
}

int CAFP::setParm(double nfs, double stepTime, 
	double dataTime, int nfftSize, int mode) {
	if(nfs <= 0.0 || stepTime <= 0.0 || 
		dataTime <= 0.0 || nfftSize <= 0 || 
		mode < 0 || mode > 2) {
		return 1;
	}
	fs = nfs;
	fftSize = nfftSize;
	dataSize = (int)(dataTime * fs);

	if(dataSize > nfftSize) {
		return 1;
	}

	hopSize = (int)(stepTime * fs);
	lastData = curData = NULL;
	lastSize = curSize = 0;
	maxLastSize = maxCurSize = 0;

	fftbuf = new double[fftSize];

	switch(mode) {
	case 0: {
		freqs = line_freqs;
		break;
	}
	case 1: {
		freqs = log_freqs;
		break;
	}
	case 2: {
		freqs = bark_freqs;
		break;
	}
	default: {
		return 1;
	}
	}

	pfft = new CPLXFFT();
	pfft->setFFTsize(fftSize);

	pwindow = new HammingWindow();
	pwindow->setWindowLength(dataSize);

	int i;
	for(i = 0; i < NUM_FREQS; ++i) {
		xovr[i] = (int)(freqs[i] * (double)fftSize / fs);
	}
	memset(be, 0, NUM_BANDS * 2 * sizeof(double));
	return 0;
}

void CAFP::cleanup() {
	dataSize = fftSize = hopSize = 0;
	fs = 5000.0;
	freqs = NULL;

	if(lastData != NULL) {
		delete [] lastData;
	}
	lastData = NULL;
	lastSize = 0;

	if(curData != NULL) {
		delete [] curData;
	}
	curData = NULL;
	curSize = 0;

	maxLastSize = maxCurSize = 0;

	if(fftbuf != NULL) {
		delete [] fftbuf;
	}
	fftbuf = NULL;

	delete pfft;
	delete pwindow;
}


int CAFP::getSubFingerprinting(short *src, int srcSize, void *fingerbuf, int maxSize) {
#ifdef CAFP_DEBUG
	FILE *fp;
	fp = fopen("fft.data", "wb");
#endif /* CAFP_DEBUG */

	prepareData(src, srcSize);
	if(curSize <= 0)
		return 0;

	int start = 0;
	int numSubFinger = 0;
	double *fb, te;
	int i, b;
	unsigned spf, *pspf;
	pspf = (unsigned *)fingerbuf;
	while(start + dataSize <= curSize && numSubFinger < maxSize) {
		memset(fftbuf, 0, fftSize * sizeof(double));
		memcpy(fftbuf, curData + start, dataSize * sizeof(double));

		// windowing
		pwindow->addWindow(fftbuf);


		pfft->fft(fftbuf);
		pfft->getABS(fftbuf);


#ifdef CAFP_DEBUG
		fwrite((void *)fftbuf, 1, fftSize * sizeof(double), fp);
#endif /* CAFP_DEBUG */

		for(b = 0; b < NUM_BANDS; ++b) {
			te = 0;
			for(i = xovr[b]; i < xovr[b + 1]; ++i) {
				te += fftbuf[i] * fftbuf[i];
			}
			be[b] = te;
		}

		spf = 0;
		for(b = 0; b < NUM_BANDS - 1; ++b) {
			if(be[b] - be[b + 1] - be_old[b] + be_old[b + 1] > 0) {
				spf = (spf << 1) | 0x01;
			}
			else {
				spf = spf << 1;
			}
		}

		memcpy(be_old, be, NUM_BANDS * sizeof(double));
		
		*pspf++ = spf;

		start += hopSize;
		++numSubFinger;

	}

	// for prepare data, alloc enough memory
	if(curSize - start > maxLastSize) {
		maxLastSize = curSize - start;
		if(lastData != NULL) {
			delete [] lastData;
			lastData = NULL;
		}

		lastData = new double[maxLastSize];
	}

	if(curSize - start > 0) {
		lastSize = curSize - start;
		memcpy(lastData, curData + start, lastSize * sizeof(double));
	}
	else {
		lastSize = 0;
	}

#ifdef CAFP_DEBUG
	fclose(fp);
#endif /* CAFP_DEBUG */

	return numSubFinger;
}

void CAFP::prepareData(short *src, int len) {
	
	if(lastSize + len > maxCurSize) {
		maxCurSize = lastSize + len;
		if(curData != NULL) {
			delete [] curData;
			curData = NULL;
		}
		curData = new double[maxCurSize];
	}
	
	curSize = lastSize + len;
	if(lastSize != 0) {
		memcpy(curData, lastData, lastSize * sizeof(double));
	}

	// copy short to double
	int i;
	short tmp;
	for(i = 0; i < len; ++i) {
		tmp = *src++;
		curData[lastSize + i] = s162double(tmp);
	}

	lastSize = 0;
}

int CAFP::calcuteNextSubFingerSize(int len) {
	int realLen = lastSize + len;
	if(realLen < dataSize) {
		return 0;
	}

	int t = (realLen - dataSize) / hopSize + 1;
	return t;
}
