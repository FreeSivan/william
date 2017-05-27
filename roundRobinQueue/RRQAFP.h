
#ifndef RRQAFP_H
#define RRQAFP_H

#include <cstdlib>
#include <cstdio>
#include <string.h>

#define RRQAFP_SAFE_DELETE_ARRAY(p) \
 { if((p) != NULL) delete [] (p); (p) = NULL;}

const int MAXAFPQ = 1000;

class RRQAFP
{
public:
	RRQAFP();
	~RRQAFP();

	bool isFull();
	bool isEmpty();

	int put(unsigned *data, int len);
	int get(unsigned *data, int maxLen);
	int getCount();
	int getAfpNum();

private:
	unsigned *p[MAXAFPQ];
	int maxSize[MAXAFPQ];
	int curSize[MAXAFPQ];
	int head, tail;
	int count;
	int afpNum;
};

#endif /* RRQAFP_H */


