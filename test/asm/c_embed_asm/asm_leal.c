#include <stdio.h>

int main(int argc, char* argv[])
{
    int i = 1;

    // eax = eax + eax * 4
    __asm__ __volatile__("leal (%1,%1,4), %1":"=a"(i):"0"(i));
    printf ("i:%d\n", i);
    return 0;
}
