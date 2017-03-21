#include <stdio.h>

// btsl 把指定bit1 置1
int main()
{
    int i = 0;
    __asm__ ("movl $0x0, %0; btsl $1, %0" : "=a"(i)::);
    printf("i:%d\n", i);

    __asm__ ("movl $0x0, %0; btsl $2, %0" : "=a"(i)::);
    printf("i:%d\n", i);

    __asm__ ("movl $0x0, %0; btsl $3, %0" : "=a"(i)::);
    printf("i:%d\n", i);

    __asm__ ("movl $0x0, %0; btsl $4, %0" : "=a"(i)::);
    printf("i:%d\n", i);

    __asm__ ("movl $0x0, %0; btsl $5, %0" : "=a"(i)::);
    printf("i:%d\n", i);
    return 0;
}
