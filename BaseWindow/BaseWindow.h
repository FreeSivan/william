/*
 * create by sivan.yue
 * data : 2017-05-27
 */

#ifndef BASE_WINDOW
#define BASE_WINDOW

class BaseWindow {
public:
	BaseWindow(){}
	virtual ~BaseWindow(){}

	virtual int setWindowLength(int nLen){ return 0; }
	virtual int addWindow(double *src){ return 0; }
	virtual void cleanup(){ }

};

#endif /* BASE_WINDOW */