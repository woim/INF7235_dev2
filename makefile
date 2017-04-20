# Makefiel pour projet resampling
COMPILER=/usr/bin/gcc
OPTIONS= -I stb/ -lm -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

default: resampling_seq
	./resampling_sequential parrot.ppm

resampling: resampling.c
	$(COMPILER) $(OPTIONS) -c resampling.c -o resampling.o

resampling_seq: resampling resampling_sequential.c
	$(COMPILER) $(OPTIONS) resampling_sequential.c -o resampling_sequential resampling.o

bm:

clean:
	\rm -f *~
