#include "resampling.h"
#include "mpi.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

void resampling_mpi1(
    double matrix[2][3],
    Image *source,
    Image *destination);
