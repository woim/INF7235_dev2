#include "resampling_mpi.h"


void resampling_mpi1(
    double matrix[2][3],
    Image* source,
    Image* destination)
{
  int id_proc;
  int nb_procs;
  int i, remainder, nb_rows, offset, origin_offset;
  int *lengths;
  int *offsets;
  unsigned char* ptr_data;
  unsigned char* ptr_resampled;
  Image* aptr_destination;

  MPI_Comm_rank ( MPI_COMM_WORLD , &id_proc );
  MPI_Comm_size ( MPI_COMM_WORLD , &nb_procs );
  
  lengths = malloc(sizeof(int)*nb_procs);
  offsets = malloc(sizeof(int)*nb_procs);
  aptr_destination = malloc(sizeof(Image)*nb_procs);
  ptr_resampled = malloc(sizeof(unsigned char)*source->length);
  
  remainder = source->size[1] % nb_procs;
  nb_rows = source->size[1] / nb_procs;
  offset = 0;
  origin_offset = 0;
 
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
    aptr_destination[i].origin[1] = origin_offset;  
    aptr_destination[i].length = aptr_destination[i].size[0]*
                                    aptr_destination[i].size[1]*
                                    aptr_destination[i].data_size;
    aptr_destination[i].offset = offset;
    offset += aptr_destination[i].length;
    origin_offset += aptr_destination[i].size[1];
    lengths[i] = aptr_destination[i].length;
    offsets[i] = aptr_destination[i].offset;
  }
  
  ptr_data = malloc(sizeof(unsigned char)*lengths[id_proc]);
  
  MPI_Scatterv( source->data, 
                lengths,
                offsets,
                MPI_BYTE, 
                ptr_data,
                lengths[id_proc], 
                MPI_BYTE, 
                0, 
                MPI_COMM_WORLD);

  Image local_image;
  local_image.size[0] = aptr_destination[id_proc].size[0];
  local_image.size[1] = aptr_destination[id_proc].size[1];
  local_image.data_size = aptr_destination[id_proc].data_size;
  local_image.origin[0] = aptr_destination[id_proc].origin[0];
  local_image.origin[1] = aptr_destination[id_proc].origin[1];
  local_image.length = aptr_destination[id_proc].length;
  local_image.offset = aptr_destination[id_proc].offset;
  local_image.data = ptr_data;

  resampling( matrix, source, &local_image);
    
  MPI_Gatherv(  local_image.data, 
                lengths[id_proc],
                MPI_BYTE,
                ptr_resampled,
                lengths,
                offsets,
                MPI_BYTE,
                0,
                MPI_COMM_WORLD);

  if( id_proc == 0 )
  {
    destination->data = ptr_resampled;
  }
  free(ptr_data);
  free(aptr_destination);
}

