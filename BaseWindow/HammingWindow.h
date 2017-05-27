/*
 * create by sivan.yue
 * data : 2017-05-27
 */


#ifndef HAMMING_WINDOW
#define HAMMING_WINDOW

#include "BaseWindow.h"

class HammingWindow : public BaseWindow {
public:
	HammingWindow();
	~HammingWindow();
	
	int setWindowLength(int nLen);
	int addWindow(double *src);
	void cleanup();
		
private:
	int window_len;
	double *winp;

};
#endif /* HAMMING_WINDOW */
