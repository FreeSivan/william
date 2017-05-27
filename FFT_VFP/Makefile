GCC = gcc
CFLAGS = -c -g
OBJS = codegen.o ffts.o ffts_nd.o ffts_real.o ffts_real_nd.o ffts_small.o ffts_static.o patterns.o

all:${OBJS}

clean:
	@rm -rf ${OBJS}

${OBJS} : %.o:%.c
	${GCC} ${CFLAGS} $< -o $@
	cp $@ ../bin/
	@rm -rf *.o
