#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    for (int i=0; i<4; ++i)
    {
        printf( "hello world. \n" );
    }
    sleep(2);
    sleep(2);
    return 0;
}
