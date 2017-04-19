#include "stdio.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

#define STB_DEFINE
#include "stb.h"

int main( int argc, char *argv[] )
{
  if( argc < 2 )
  {
    printf("%s filename\n",argv[0]);
    return 0;
  }
  printf("Loading file: %s\n",argv[1]);
  int x,y,n;
  unsigned char *data = stbi_load( argv[1], &x, &y, &n, 0 );
  printf("Width: %d\n",x);
  printf("Height: %d\n",y);
  printf("DataType: %d\n",n);
 
  return 0;
}
