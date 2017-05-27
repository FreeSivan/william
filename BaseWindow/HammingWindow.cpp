/*
 * create by sivan.yue
 * data : 2017-05-27
 */

#include <cstdlib>
#include <cmath>

#include "HammingWindow.h"

#define dpi 6.283185307179586
#define ha 0.53836 
#define hb 0.46164

HammingWindow::HammingWindow():
		window_len(0), winp(NULL) {
}

HammingWindow::~HammingWindow() {
	cleanup();
}

int HammingWindow::setWindowLength(int nLen) {
	if(nLen <= 0)
		return 1;

	window_len = nLen;
	winp = new double[window_len];

	int i;
	for(i = 0; i < window_len; ++i)
		winp[i] = ha - hb * cos(dpi * double(i) / double(window_len - 1.0));

	return 0;
}

int HammingWindow::addWindow(double *src) {
	if(src == NULL || winp == NULL)
		return 1;

	int i;
	for(i = 0; i < window_len; ++i)
	{
		src[i] *= winp[i];
	}


	return 0;
}


void HammingWindow::cleanup() {
	if(winp != NULL)
		delete [] winp;

	winp = NULL;
	window_len = 0;
}