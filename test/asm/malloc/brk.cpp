#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    unsigned int pos = 0;
    unsigned int pos2 = 0;
    pos = (unsigned int)sbrk(0);

    // 分配4K
    int ret = brk((char*)pos + 4*1024);
    if (ret)
    {
        printf("brk error");
        return -1;
    }

    pos2 = (unsigned int)sbrk(0);

    printf("pos  : %u,%X\n", pos, pos);
    printf("pos2 : %u,%X\n", pos2, pos2);
    printf("pos2-pos : %u,%X\n", pos2-pos, pos2-pos);

    // 如果越界超过4K,则coredump
    //memset(pstr, 0, 1024*4 + 1);
    return 0;
}
