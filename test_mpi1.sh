#!/bin/bash

./resampling_sequential parrot.ppm
mpirun -np 2 resampling_parallel parrot.ppm

diff=$(diff *.bmp)

if [ "$diff" == "" ]; then
  echo "Test mpi1: PASS"
else
  echo "Test mpi1: FAIL"
fi

rm -f *~
rm -f *.o
rm -rf *.bmp
rm -f resampling_sequential test_resampling resampling_parallel
