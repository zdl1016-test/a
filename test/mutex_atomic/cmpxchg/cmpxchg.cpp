#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>
#include <sstream>
#include <vector>
#include <map>
#include <unistd.h> // for usleep

using namespace std;


#ifndef foreach
#define foreach(container,it) \
            for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

int test_ngx_atomic_cmp_set(int old, int set, int *addr)
{
    u_char  res;

    __asm__ volatile (

         "lock;"
    "    cmpxchgl  %3, %1;   "
    "    sete      %0;       "

    : "=a" (res) : "m" (*addr), "a" (old), "r" (set) : "cc", "memory");

    return res;
}

int test_android_cmp_set(int old_val, int new_val, volatile int * addr)
{
    int xchg;
    asm volatile
    (
        "lock; cmpxchg %%ecx, (%%edx);"
        "sete %%al;"
        "andl $1, %%eax;"
        : "=a" (xchg)
        : "a" (old_val), "c" (new_val), "d" (addr)

    );
    return xchg;
}


int main()
{
    int a = 5;

    int ret = 0;
    ret = test_ngx_atomic_cmp_set(5, 6, &a);
    printf("a:%d, ret:%d\n", a, ret);

    a = 5;
    ret = test_android_cmp_set(5,6,&a);
    printf("a:%d, ret:%d\n", a, ret);

    return 0;
}
