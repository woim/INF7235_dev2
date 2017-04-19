#include <iostream>

#include "CImg.h"
using namespace cimg_library;


int main( int argc, char *argv[] )
{
  if( argc < 2 )
  {
    std::cout << argv[0] << " filename" << std::endl;
    return 0;
  }
  std::cout << "Loading file: " << argv[1] << std::endl;

  CImg<unsigned char> image( argv[1] );
  std::cout << "Width: " << image.width() << std::endl;
  std::cout << "Height: " << image.height() << std::endl;
  return 0;
}
