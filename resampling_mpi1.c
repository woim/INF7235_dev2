#include "resampling_mpi.h"

void resampling_mpi1(
    double matrix[2][3],
    Image *source,
    Image *destination)
{
  int id_proc;
  int nb_procs;
  int i, nb_rows;
  Image* ptr_image;
  Image** aptr_resampled;
  Image** aptr_destination;

/*
  MPI_Comm_rank ( MPI_COMM_WORLD , &id_proc );
  MPI_Comm_size ( MPI_COMM_WORLD , &nb_procs );
*/

  nb_rows = source->size[1]/nb_procs;
  aptr_resampled = malloc(sizeof(Image*)*nb_procs);
  aptr_destination = malloc(sizeof(Image*)*nb_procs);
  ptr_image = malloc(sizeof(Image));

  for(i = 0; i < nb_procs ; ++i)
  {
    aptr_destination[i] = malloc(sizeof(Image));
    aptr_destination[i]->size[0] = source->size[0];
    aptr_destination[i]->size[1] = nb_rows;
    aptr_destination[i]->origin[0] = source->origin[0];
    aptr_destination[i]->origin[1] = source->origin[1]+i*nb_rows;
    aptr_destination[i]->data_size = source->data_size;
    aptr_destination[i]->data = NULL;
  }

/*
  //MPI_Bcast ( source , sizeof(Image) , MPI_BYTE , 0 , MPI_COMM_WORLD );
  //MPI_Broadcast(source);
  //MPI_Broadcast(matrix);


  MPI_Scatter( aptr_source , sizeof(Image) , MPI_BYTE , ptr_image , sizeof(Image) , MPI_BYTE , 0 , MPI_COMM_WORLD );
  //resampling( matrix, source, ptr_image );
  printf("Proceseeur: %d value ptr: %d", id_proc, ptr_image);
  printf("Size: %d %d", ptr_image->size[0], ptr_image->size[1]);
  MPI_Gather ( ptr_image , sizeof(Image) , MPI_BYTE , aptr_resampled , sizeof(Image) , MPI_BYTE , 0 , MPI_COMM_WORLD );
*/

  for(i = 0; i < nb_procs ; ++i)
  {
    stbi_image_free(aptr_destination[i]->data);
    stbi_image_free(aptr_resampled[i]->data);
    free(aptr_destination[i]);
    free(aptr_resampled[i]);
  }
  free(aptr_destination);
  free(aptr_resampled);
  free(ptr_image);
}
