GCC = g++
CFLAGS = -c -g -std=c++0x

OBJS = CAFP.o

all:${OBJS}

clean:
	@rm -rf ${OBJS}

${OBJS} : %.o:%.cpp
	${GCC} ${CFLAGS} $< -o $@
	cp $@ ../bin 
	@rm -rf *.o
