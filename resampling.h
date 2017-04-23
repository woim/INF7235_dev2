#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "memory.h" 

typedef struct {
  int size[2];
  int origin[2];
  int data_size;
  int length;
  int offset;
  unsigned char* data;
}Image;

void transform_point(
  double matrix[2][3],
  double* coord,
  double* new_coord);

void interpolate(
    Image* source,
    double wcoord[2],
    unsigned char* new_value);

void set_pixel(
    Image* destination,
    int coord[2],
    unsigned char new_value[3]);

void resampling(
  double matrix[2][3],
  Image* source,
  Image* destination);
