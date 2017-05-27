GCC = g++
CFLAGS = -c -g -std=c++0x

OBJS = CPLXFFT.o CPLXFFT_R24_F32.o

all:${OBJS}

clean:
	@rm -rf ${OBJS}

${OBJS} : %.o:%.cpp
	${GCC} ${CFLAGS} $< -o $@
	cp $@ ../bin/
	@rm *.o -f 

