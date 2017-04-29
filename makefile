# Makefiel pour projet resampling
COMPILER=/usr/bin/gcc
MPI_COMPILER=mpicc -std=c99
OPTIONS= -I stb/ -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
IMAGE=parrot.ppm

run_seq: resampling_seq
	./resampling_sequential $(IMAGE)

run_par: resampling_par
	mpirun -np ${N} resampling_parallel $(IMAGE)
	
test: test_comp test_mpi1

resampling: resampling.c
	$(COMPILER) $(OPTIONS) -c resampling.c -o resampling.o

resampling_seq: resampling resampling_sequential.c
	$(COMPILER) $(OPTIONS) -lm resampling_sequential.c -o resampling_sequential resampling.o

resampling_mpi1: resampling_mpi1.c resampling
	$(MPI_COMPILER) $(OPTIONS) -c resampling_mpi1.c -o resampling_mpi1.o

resampling_par: resampling_mpi1 	 
	$(MPI_COMPILER) ${OPTIONS} resampling_parallel.c -o resampling_parallel resampling_mpi1.o resampling.o

test_comp: test_res
	./test_resampling

test_res: resampling test_resampling.c
	$(COMPILER) $(OPTIONS) test_resampling.c -o test_resampling resampling.o

test_mpi1: resampling_seq resampling_par
	./test_mpi1.sh

bm1: resampling_seq resampling_par
	./performance_mpi1.pl

clean:
	\rm -f *~
	\rm -f *.o
	\rm -f resampling_sequential test_resampling resampling_parallel
