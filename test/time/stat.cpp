#include <time.h>
#include "ctimer.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int foo(int i)
{
    return i;
}

const int MAX_COUNT = 1000*1000;

int main()
{
    CMyTimer t;
    struct timeval tpTmp; 

    printf("repeat %d times, test result is : \n", MAX_COUNT);
    printf("\n");

    {
        t.Begin();
        for (int i=0; i<MAX_COUNT; ++i)
            foo(i);

        printf("foo():\n");
        printf("elapse : %5.5f sec\n", t.GetElapseTimeSec()); 
        printf("\n");
    }

    {
        t.Begin();
        for (int i=0; i<MAX_COUNT; ++i)
            time(NULL);;

        printf("time():\n");
        printf("elapse : %5.5f sec\n", t.GetElapseTimeSec());
        printf("\n");
    }

    {
        t.Begin();
        for (int i=0; i<MAX_COUNT; ++i)
            gettimeofday(&tpTmp, NULL);;

        printf("gettimeofday():\n");
        printf("elapse : %5.5f sec\n", t.GetElapseTimeSec());
        printf("\n");
    }


    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        t.Begin();
        for (int i=0; i<MAX_COUNT; ++i)
            stat("1.txt", &st);

        printf("stat():\n");
        printf("elapse : %5.5f sec\n", t.GetElapseTimeSec());
        printf("\n");
    }
    return 0;
}
