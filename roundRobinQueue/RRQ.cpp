
#include "RRQ.h"

RRQ::RRQ(): head(0), tail(0), count(0) {
	int i;
	for(i = 0; i < MAXQ; ++i) {
		p[i] = NULL;
		maxSize[i] = 0;
		curSize[i] = 0;
	}
}

bool RRQ::isFull() {
	return count == MAXQ;
}

bool RRQ::isEmpty() {
	return count == 0;
}

int RRQ::put(short *data, int len) {
	if(isFull()) {
		return 1;
	}

	if(len > maxSize[tail]) {
		RRQ_SAFE_DELETE_ARRAY(p[tail]);

		maxSize[tail] = len;
		p[tail] = new short[len];
	}

	memcpy(p[tail], data, len * sizeof(short));
	curSize[tail] = len;

	tail = (tail + 1) % MAXQ;
	++count;

	return 0;
}

short * RRQ::get(int &len) {
	if(isEmpty()) {
		len = 0;
		return NULL;
	}

	short *tp = p[head];
	len = curSize[head];
	head = (head + 1) % MAXQ;
	--count;

	return tp;
}

RRQ::~RRQ() {
	int i;
	for(i = 0; i < MAXQ; ++i) {
		RRQ_SAFE_DELETE_ARRAY(p[i]);
	}

	count = 0;
	head = tail = 0;
}

int RRQ::getCount() {
	return count;
}