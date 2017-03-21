#include <stdio.h>

// stos b/w/l 字节copy, 能实现memmet的效果
int main(int argc, char* argv[])
{
    char * p = "hello";
    char buf[100] = {0};
    int count = 6;
    int i = 0;

    //--------------------------------
    
    snprintf(buf, sizeof(buf), "%s", "sssssssss");
    printf("buf : [%s]\n", buf);

    __asm__ (
            "cld\n\t"
            "rep\n\t"
            "stosl"
            :
            :"c"(1), "a"(0), "D"(buf)
            );

    for (i=0; i<5; i++)
    {
        printf("buf i:%d: [%s]\n", i, &buf[i]);
    }


    //--------------------------------
    
    
    snprintf(buf, sizeof(buf), "%s", "sssssssss");

    __asm__ (
            "cld\n\t"
            "rep\n\t"
            "stosw"
            :
            :"c"(1), "a"(0), "D"(buf)
            );

    for (i=0; i<5; i++)
    {
        printf("buf i:%d: [%s]\n", i, &buf[i]);
    }

    //--------------------------------
    
    
    snprintf(buf, sizeof(buf), "%s", "sssssssss");

    __asm__ (
            "cld\n\t"
            "rep\n\t"
            "stosb"
            :
            :"c"(1), "a"(0), "D"(buf)
            );

    for (i=0; i<5; i++)
    {
        printf("buf i:%d: [%s]\n", i, &buf[i]);
    }
    return 0;
}
