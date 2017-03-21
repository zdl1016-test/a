#include <stdio.h>

int main()
{
    int len = 0;
    unsigned int num = 1;
    for (int i=0; i<32; i++)
    {
        __asm__ ("bsfl %1, %0":"=r"(len):"r"(num):);
        printf("num:%u, first non-0 bit index(from low bit to high bit, start 0):%d\n", num, len);
        num <<= 1;
    }

    // warning 
    num = 0;
    __asm__ ("bsfl %1, %0":"=r"(len):"r"(num):);
    printf("num:%u, first non-0 bit index(from low bit to high bit, start 0):%d\n", num, len);
    return 0;
}
