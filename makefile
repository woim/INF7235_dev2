# Makefiel pour projet resampling
COMPILER=/usr/bin/gcc
OPTIONS= -I stb/ -lm -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

default: resampling_seq
	./resampling_sequential parrot.ppm

test: test_res
	./test_resampling

resampling: resampling.c
	$(COMPILER) $(OPTIONS) -c resampling.c -o resampling.o

resampling_seq: resampling resampling_sequential.c
	$(COMPILER) $(OPTIONS) resampling_sequential.c -o resampling_sequential resampling.o

test_res: test_resampling.c resampling
	$(COMPILER) $(OPTIONS) test_resampling.c -o test_resampling resampling.o

bm:

clean:
	\rm -f *~
	\rm -f *.o
	\rm -f resampling_sequential test_resampling
