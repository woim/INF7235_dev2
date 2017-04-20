#include "stddef.h"

typedef struct {
  int size[2];
  int origin[2];
  int data_size;
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

void transform_point(
    double matrix[2][3],
    double* coord,
    double* new_coord);
