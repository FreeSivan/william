GCC = g++
CFLAGS = -c -g -std=c++0x

OBJS = FastMemory.o

all:${OBJS}

clean:
	@rm -rf ${OBJS}

${OBJS} : %.o:%.cpp
	${GCC} ${CFLAGS} $< -o $@
	cp $@ ../bin/
	@rm *.o -rf
