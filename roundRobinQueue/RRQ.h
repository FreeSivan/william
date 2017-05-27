
#ifndef RRQ_H
#define RRQ_H

#include <cstdlib>
#include <cstdio>
#include <string.h>

#define RRQ_SAFE_DELETE_ARRAY(p) \
 { if((p) != NULL) delete [] (p); (p) = NULL;}

const int MAXQ = 100;

class RRQ {
public:
	RRQ();
	~RRQ();

	bool isFull();
	bool isEmpty();

	int put(short *data, int len);
	short *get(int &len);
	int getCount();

private:
	short *p[MAXQ];
	int maxSize[MAXQ];
	int curSize[MAXQ];
	int head, tail;
	int count;
};

#endif /* RRQ_H */