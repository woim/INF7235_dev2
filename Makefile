# Makefiel pour projet resampling
COMPILER=/usr/bin/gcc
OPTIONS= -I stb/ -lm -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

default: resampling_seq
	./resampling_sequential parrot.ppm

resampling_seq: resampling_sequential.c
	$(COMPILER) $(OPTIONS) resampling_sequential.c -o resampling_sequential

bm:

clean:
	\rm -f resampling*
	\rm -f *~
