# Makefiel pour projet resampling
COMPILER=/usr/bin/g++
OPTIONS= -std=c++11 -Wall -Wfatal-errors -Dcimg_display=0

default: resampling_seq
	./resampling_sequential parrot.ppm

resampling_seq: resampling_sequential.cpp
	$(COMPILER) $(OPTIONS) resampling_sequential.cpp -o resampling_sequential

bm:

clean:
	\rm -f resampling*
	\rm -f *~
