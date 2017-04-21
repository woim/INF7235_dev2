#include "resampling.h"

int test_transform()
{
  double coord1[2] = {1.0, 1.0};
  double coord2[2] = {0.0, 0.0};
  double coord_expected[2] = {10.5, 15.5};
  double matrix[2][3] = { {0.5, 0.0, 10.0},
                          {0.0, 0.5, 15.0} };
  transform_point(matrix, &coord1[0], &coord2[0]);
  return (coord2[0] == coord_expected[0] && coord2[1] == coord_expected[1]) ? 1 : 0;
}

int test_set_pixel()
{
  int coord[2] = {0, 0};
  unsigned char new_value[3] = {0, 128, 255};
  Image img;
  img.size[0] = 0;
  img.size[1] = 0;
  img.data_size = 3;
  img.data = (unsigned char*) malloc(3*sizeof(unsigned char));
  set_pixel(&img, coord, new_value);
  return (img.data[0] == new_value[0] &&
          img.data[1] == new_value[1] &&
          img.data[2] == new_value[2]) ? 1 : 0;
}

int test_interpolate()
{
  int i;
  double wcoord[2] = {0.5, 0.5};
  unsigned char value_expected[3] = {191, 191, 191};
  unsigned char new_value[3];
  Image img;
  img.size[0] = img.size[1] = 2;
  img.origin[0] = img.origin[0] = 0;
  img.data_size = 3;
  img.data = (unsigned char*) malloc(12*sizeof(unsigned char));
  for(i = 0; i < 12; ++i)
  {
    if( i < 6 ) img.data[i] = 255;
    if( i > 5 ) img.data[i] = 128;
  }
  interpolate(&img, wcoord, &new_value[0]);
  return ( value_expected[0] == new_value[0] &&
           value_expected[1] == new_value[1] &&
           value_expected[2] == new_value[2]) ? 1 : 0;
}

void run_test(int test, char* test_name)
{
  char* result;
  char* message;
  result = (test != 0) ? " : PASS" : " : FAIL";
  message = (char *) malloc(1 + strlen(test_name)+ strlen(result) );
  strcpy(message, test_name);
  strcat(message, result);
  printf("%s\n", message);
}

int main(int argc, char* argv[])
{
  run_test(test_transform(), "Test transform");
  run_test(test_set_pixel(), "Test set_pixel");
  run_test(test_interpolate(), "Test interpolate");
  return 0;
}
