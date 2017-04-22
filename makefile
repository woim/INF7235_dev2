# Makefiel pour projet resampling
COMPILER=/usr/bin/gcc
MPI_COMPILER=mpicc
OPTIONS= -std=c99 -I stb/ -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

run_seq: resampling_seq
	./resampling_sequential parrot.ppm

run_par: resampling_par
	mpirun -np 8 resampling_parallel

test: test_res
	./test_resampling

resampling: resampling.c
	$(COMPILER) $(OPTIONS) -c resampling.c -o resampling.o

resampling_seq: resampling resampling_sequential.c
	$(COMPILER) $(OPTIONS) resampling_sequential.c -o resampling_sequential resampling.o

resampling_mpi1: resampling resampling_mpi1.c
	$(MPI_COMPILER) $(OPTIONS) -c resampling_mpi1.c -o resampling_mpi1.o resampling.o

resampling_par: resmapling_mpi1 resampling
	$(MPI_COMPILER) $(OPTIONS) -c resampling_parallel.c -o resampling_parallel resampling_mp1.o resampling.o

test_res: test_resampling.c resampling
	$(COMPILER) $(OPTIONS) test_resampling.c -o test_resampling resampling.o

bm:

clean:
	\rm -f *~
	\rm -f *.o
	\rm -f resampling_sequential test_resampling
