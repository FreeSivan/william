
#include "RRQAFP.h"

RRQAFP::RRQAFP():head(0), tail(0),count(0), afpNum(0) {
	int i;
	for(i = 0; i < MAXAFPQ; ++i) {
		p[i] = NULL;
		maxSize[i] = 0;
		curSize[i] = 0;
	}
}

bool RRQAFP::isFull() {
	return count == MAXAFPQ;
}

bool RRQAFP::isEmpty() {
	return count == 0;
}

int RRQAFP::put(unsigned *data, int len) {
	if(isFull() || len <= 0) {
		return 1;
	}

	if(len > maxSize[tail]) {
		RRQAFP_SAFE_DELETE_ARRAY(p[tail]);
		maxSize[tail] = len;
		p[tail] = new unsigned[len];
	}

	memcpy(p[tail], data, len * sizeof(unsigned));
	curSize[tail] = len;

	tail = (tail + 1) % MAXAFPQ;
	++count;
	afpNum += len;

	return 0;
}

int RRQAFP::get(unsigned *data, int maxLen) {
	if(isEmpty()) {
		return 0;
	}

	int realLen = maxLen > curSize[head] ? curSize[head] : maxLen;
	memcpy(data, p[head], realLen * sizeof(unsigned));

	if(realLen < curSize[head]) {
		memcpy(p[head], p[head] + realLen,
			(curSize[head] - realLen) * sizeof(unsigned));
		curSize[head] -= realLen;
	}
	else {
		head = (head + 1) % MAXAFPQ;
		--count;
	}

	afpNum -= realLen;

	return realLen;
}

RRQAFP::~RRQAFP() {
	int i;
	for(i = 0; i < MAXAFPQ; ++i) {
		RRQAFP_SAFE_DELETE_ARRAY(p[i]);
	}

	count = afpNum = 0;
	head = tail = 0;
}

int RRQAFP::getCount() {
	return count;
}

int RRQAFP::getAfpNum() {
	return afpNum;
}