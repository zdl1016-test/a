#include <stdio.h>
extern inline char * strcpy2(char * dest,const char *src);
int main()
{
    char * p = "abc";
    char buf[128] = {0};

    strcpy2(buf, p);

    printf("buf:[%s]", buf);
}
char * strcpy2(char * dest,const char *src)
{
    __asm__("cld\n"
            "1:\tlodsb\n\t"
            "stosb\n\t"
            "testb %%al,%%al\n\t"
            "jne 1b"
            ::"S" (src),"D" (dest):"si","di","ax");
    return dest;
}

