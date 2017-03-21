#include <stdio.h>

int print_binary(unsigned int n)
{
    unsigned int mask = 0x80000000;
    for (int i=0; i<32; ++i)
    {
        if (n & mask)
            printf("1");
        else
            printf("0");

        n<<=1;
    }
    printf("\n");
}
// btsl 把指定bit 置o
int main()
{
    int i = 0xFF;
    __asm__ ("btrl $0, %0" : "=a"(i):"0"(i):);
    printf("i:%d\n", i);
    print_binary(i);

    i = 0xFF;
    __asm__ ("btrl $1, %0" : "=a"(i):"0"(i):);
    printf("i:%d\n", i);
    print_binary(i);

    i = 0xFF;
    __asm__ ("btrl $2, %0" : "=a"(i):"0"(i):);
    printf("i:%d\n", i);
    print_binary(i);
    return 0;
}
