#include <stdio.h>

int main()
{
    fprintf( stdout, "from stdout 1\n" );
    fprintf( stderr, "from stderr 1\n" );
    fprintf( stderr, "from stderr 2\n" );
    fprintf( stdout, "from stdout 2\n" );
    return 0;
}
