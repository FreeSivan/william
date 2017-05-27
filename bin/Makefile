GCC = g++
CFLAGS = -c -g -std=c++0x

OBJS = CAFP.o CPLXFFT_R24_F32.o finger.o RRQ.o CPLXFFT.o FastMemory.o HammingWindow.o  RRQAFP.o

all:
	cd ../BaseFFT; make
	cd ../BaseWindow; make
	cd ../CAFP; make
	cd ../roundRobinQueue; make
	cd ../Shared; make
	cd ../finger; make
	$(GCC) -o finger $(OBJS)

clean:
	cd ../BaseFFT; make clean
	cd ../BaseWindow; make clean
	cd ../CAFP; make clean
	cd ../roundRobinQueue; make clean
	cd ../Shared; make clean
	cd ../finger; make clean
	@rm *.o finger -f

