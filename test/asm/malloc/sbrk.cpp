#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    unsigned int pos = 0;
    pos = (unsigned int)sbrk(0);

    printf("init pos : %u,%X\n", pos, pos);
    
    // 分配内存
    char * pstr = (char*)sbrk(1);

    pos = (unsigned int)sbrk(0);
    printf("pos : %u,%X\n", pos, pos);

    // 如果越界超过4K,则coredump
    //memset(pstr, 0, 1024*4 + 1);
    return 0;
}
