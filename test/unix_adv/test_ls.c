#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>


int main( int argc, char* argv[] )
{
  DIR *dp;
  struct dirent *dirp;

  if( argc != 2 )
  {
    printf(" a single argument is required");
  }

  if( (dp=opendir(argv[1])) == 0 )
  {
    printf( "cant not open %s", argv[1] );
  }  

  while( (dirp = readdir(dp))!=0 )
    printf( "%s\n", dirp->d_name );

  closedir( dp );
  return 0;
}
