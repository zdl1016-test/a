/* time example */
#include <stdio.h>
#include <time.h>

#define SECS_A_DAY (3600*24)

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

#ifndef foreach
#define foreach(container,it) \
        for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

#undef BYTE_COPY_FWD
#define BYTE_COPY_FWD(dst, src, len)       \
        do { \
                    asm volatile(                       \
                                            "cld\n"                     \
                                            "rep\n"                     \
                                            "movsb" :                   \
                                            "=D" (dst), "=S" (src) :    \
                                            "0" (dst), "1" (src), "c" (len) : \
                                            "memory");                  \
                } while (0)

void *memcpy (void *dst, void *src, size_t len)
{
    char * pdst = (char*)dst;
    char * psrc = (char*)src;

    if (len >= sizeof(long))
    {
        // 先字节对齐
        BYTE_COPY_FWD(pdst, psrc, len % sizeof(long));
        len -= len % sizeof(long);

        // 再一次copy一个字长
        //WORD_COPY_FWD(pdst, psrc, len, len);
        //BYTE_COPY_FWD(pdst, psrc, len, len);
    }
    BYTE_COPY_FWD(pdst, psrc, len);

    return dst;
}

int main ()
{
    char c1[20] =  {0};
    snprintf(c1, sizeof(c1), "hello world");
    char c2[20] =  {0};
    memcpy (c2, c1, 10);

    printf("c1 : %s\n", c1);
    printf("c2 : %s\n", c2);
    

    return 0;
}

