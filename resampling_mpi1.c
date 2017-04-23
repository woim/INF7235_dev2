#include "resampling_mpi.h"

void resampling_mpi1(
    double matrix[2][3],
    Image *source,
    Image *destination)
{
  int id_proc;
  int nb_procs;
  int i, remainder, nb_rows;
  Image* ptr_image;
  Image* aptr_resampled;
  Image* aptr_destination;

  MPI_Comm_rank ( MPI_COMM_WORLD , &id_proc );
  MPI_Comm_size ( MPI_COMM_WORLD , &nb_procs );

  ptr_image = malloc(sizeof(Image));
  aptr_resampled = malloc(sizeof(Image)*nb_procs);

  if( id_proc == 0 )
  {
    remainder = source->size[1] % nb_procs;
    nb_rows = source->size[1] / nb_procs;
    aptr_destination = malloc(sizeof(Image)*nb_procs);
    
    for(i = 0; i < nb_procs ; ++i)
    {
      aptr_destination[i].size[0] = source->size[0];
      aptr_destination[i].size[1] = nb_rows;
      if( remainder > 0 )
      {
        aptr_destination[i].size[1]++;
        remainder--;
      } 
      aptr_destination[i].data_size = source->data_size;
      aptr_destination[i].origin[0] = source->origin[0];
      aptr_destination[i].origin[1] = source->origin[1]+i*nb_rows;
      aptr_destination[i].data = NULL;
   }
  }
  
  MPI_Scatter(  aptr_destination, 
                sizeof(Image), 
                MPI_BYTE, 
                ptr_image, 
                sizeof(Image), 
                MPI_BYTE, 
                0, 
                MPI_COMM_WORLD);
 
  //resampling( matrix, source, ptr_image );
  printf("Proceseeur: %d value ptr: %d\n", id_proc, ptr_image);
  printf("Size: %d %d %d\n", ptr_image->size[0], ptr_image->size[1], ptr_image->data_size);
  
  MPI_Gather (  ptr_image, 
                sizeof(Image), 
                MPI_BYTE,
                aptr_resampled,
                sizeof(Image),
                MPI_BYTE,
                0,
                MPI_COMM_WORLD );

  if( id_proc == 0 )
  {
    for(i = 0; i < nb_procs ; ++i)
    {
      //stbi_image_free(aptr_destination[i].data);
      //stbi_image_free(aptr_resampled[i].data);
    }
 
    free(aptr_destination);
    free(aptr_resampled);
    free(ptr_image);
  }
}
