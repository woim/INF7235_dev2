#include "stddef.h"

typedef struct {
  int size[2];
  int origin[2];
  int data_size;
  unsigned char* data;
}Image;

void resampling(
  double matrix[2][3], 
  Image* source,
  Image* destination);

void transform_point(
    double matrix[2][3],
    double* coord,
    double* new_coord);
