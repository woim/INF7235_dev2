#include "stdio.h"
#include "stddef.h"
#include "resampling_mpi.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main(int argc, char* argv[])
{
  int x, y, n, i, length, id_proc, nb_procs;
  unsigned char* data_src;
  Image source, destination;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);

  if( argc < 2 )
  {
    printf("%s filename\n", argv[0]);
    return 0;
  }  

  // Laod the image
  data_src = stbi_load(argv[1], &x, &y, &n, 0);

  if( data_src == NULL )
  {
    printf("Error: %s\n", stbi_failure_reason());
    return 0;
  }

  printf("Width: %d\n", x);
  printf("Height: %d\n", y);
  printf("Data length: %d\n", n);

  source.size[0] = x;
  source.size[1] = y;
  source.origin[0] = 0;
  source.origin[1] = 0;
  source.data_size = n;
  source.data = data_src;

  destination.size[0] = source.size[0];
  destination.size[1] = source.size[1];
  destination.origin[0] = source.origin[0];
  destination.origin[1] = source.origin[1];
  destination.data_size = source.data_size;
  destination.data = (unsigned char*) malloc(
                                        destination.size[0]*
                                        destination.size[1]*
                                        destination.data_size*
                                        sizeof(unsigned char));
  
  double matrix[2][3] = { {0.86, -0.5, 50.0},
                          {0.5, 0.86, 50.0} };

  printf("%d %d\n", nb_procs, id_proc);
  MPI_Barrier(MPI_COMM_WORLD);
  resampling_mpi1( matrix, &source, &destination );
  
  if( destination.data == NULL )
  {
    printf("Error: resampling failed\n");
    return 0;
  }

  stbi_write_bmp("test_mpi1.bmp",
      destination.size[0],
      destination.size[1],
      destination.data_size,
      destination.data);

  stbi_image_free(destination.data);
  stbi_image_free(source.data);
  MPI_Finalize();
  return 0;
}
