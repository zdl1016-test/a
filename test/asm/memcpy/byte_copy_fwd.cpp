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
#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes)                     \
    do {                                        \
        int __d0;                                   \
        asm volatile(/* Clear the direction flag, so copying goes forward.  */    \
                "cld\n"                              \
                /* Copy bytes.  */                       \
                "rep\n"                              \
                "movsb" :                            \
                "=D" (dst_bp), "=S" (src_bp), "=c" (__d0) :              \
                "0" (dst_bp), "1" (src_bp), "2" (nbytes) :           \
                "memory");                           \
    } while (0)

int main ()
{
    char c1[20] =  {0};
    snprintf(c1, sizeof(c1), "hello world");
    
    char * p1 = c1;
    char * p2 = c1+3;
    BYTE_COPY_FWD(p1, p2, 3);
    printf("c1:%s\n",c1);
    printf("p1:%s\n",p1);
    printf("p2:%s\n",p2);
    printf("\n");

    return 0;
}

