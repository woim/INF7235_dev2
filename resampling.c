#include "resampling.h"


void transform_point(
  double matrix[2][3],
  double* coord,
  double* new_coord)
{
  int i,j;
  double coord_h[3];
  
  for(i = 0 ; i < 2 ; ++i)
  {
    coord_h[i] = coord[i];
    new_coord[i] = 0.0;
  }
  coord_h[2] = 1.0;
  
  for(i = 0 ; i < 2 ; ++i)
  {
    for(j = 0 ; j < 3 ; ++j)
    {
      new_coord[i] += matrix[i][j]*coord_h[j]; 
    } 
  }  
}


void interpolate(
    Image* source,
    double wcoord[2],
    unsigned char* new_value)
{
  int i,j;
  double neighbor[4];
  int coord[2];
  coord[0] = (int)wcoord[0];
  coord[1] = (int)wcoord[1];

  for(i = 0; i < 3; ++i)
  {
    int index0 = coord[1]*source->size[0]*source->data_size + coord[0]*source->data_size + i;
    neighbor[0] = (double)source->data[index0];
    int index1 = (coord[1]+1)*source->size[0]*source->data_size + coord[0]*source->data_size + i;
    neighbor[1] = (double)source->data[index1];
    int index2 = coord[1]*source->size[0]*source->data_size + (coord[0]+1)*source->data_size + i;
    neighbor[2] = (double)source->data[index2]; 
    int index3 = (coord[1]+1)*source->size[0]*source->data_size + (coord[0]+1)*source->data_size + i;
    neighbor[3] = (double)source->data[index3];    
 
    double fxy1 = (double)((coord[0]+1) - wcoord[0])*neighbor[0] + (wcoord[0] - (double)coord[0])*neighbor[2];
    double fxy2 = (double)((coord[0]+1) - wcoord[0])*neighbor[1] + (wcoord[0] - (double)coord[0])*neighbor[3];

    double value = (double)((coord[1]+1) - wcoord[1])*fxy2 + (wcoord[1] - (double)coord[1])*fxy1;

    new_value[i] = (value > 255.0) ? 255.0 : (unsigned char)value;
    new_value[i] = (value < 0.0) ? 0.0 : (unsigned char)value; 
  }
}


void set_pixel(
    Image* destination,
    int coord[2],
    unsigned char new_value[3])
{
  int i, index;

  for(i = 0; i < 3; ++i)
  {
    index = coord[1]*destination->size[0]*destination->data_size + coord[0]*destination->data_size;
    destination->data[index+i] = new_value[i];
  } 
}


void resampling( 
  double matrix[2][3],
  Image* source,
  Image* destination)
{
  int i, j, k, index;  
  int vcoord[2];
  double wcoord[2];
  double new_coord[2];
  unsigned char new_value[3];

  int end;

  for(i = 0; i < destination->size[0]; ++i)
  {
    for(j = 0; j < destination->size[1]; ++j)
    {
      new_value[0] = new_value[1] = new_value[2] = 0;

      vcoord[0] = i;
      vcoord[1] = j;
      wcoord[0] = (double)destination->origin[0] + (double)vcoord[0];
      wcoord[1] = (double)destination->origin[1] + (double)vcoord[1];
      transform_point( matrix, &wcoord[0], &new_coord[0]);

      if( new_coord[0] >= (double)source->origin[0] && 
          new_coord[0] < (double)source->size[0]   &&
          new_coord[1] >= (double)source->origin[1] && 
          new_coord[1] < (double)source->size[1] ) 
      {
        interpolate(source, new_coord, &new_value[0]);
      }
      set_pixel(destination, vcoord, new_value);
    }
  }
}
 

