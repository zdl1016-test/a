#include "malloc.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    unsigned int base = (unsigned int)sbrk(0);
    printf("&base:%X %u\n", (unsigned int)&base, (unsigned int)&base);
    printf("bs:%X %u\n", base, base);

    char *p1 = (char*)dlmalloc(1);
    char *p2 = (char*)dlmalloc(20);
    //char *p3 = (char*)dlmalloc(300*4*1024);
    //char *p4 = (char*)dlmalloc(300*1024*1024);
    //char *p5 = (char*)dlmalloc(300*1024*1024);
    char *p5 = (char*)dlmalloc(300);

    //printf("p1:%X %u\n", (unsigned)p1, (unsigned)p1);
    //printf("p2:%X %u\n", (unsigned)p2, (unsigned)p2);
    //printf("p3:%X %u\n", (unsigned)p3, (unsigned)p3);
    //printf("p4:%X %u\n", (unsigned)p4, (unsigned)p4);
    //printf("p5:%X %u\n", (unsigned)p5, (unsigned)p5);
    //dlfree(p1);
    dlfree(p2);
    char *p6 = (char*)dlmalloc(20);
    //dlfree(p3);
    //dlfree(p4);
    dlfree(p5);
    return 0;
}
