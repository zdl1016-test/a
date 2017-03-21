#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    printf("xxxx\n");
    char * pstr = (char*)malloc(128*1024);

    // 如果越界超过4K,则coredump
    memset(pstr, 0, 1024*4 + 1);
    return 0;
}
