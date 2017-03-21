#include <time.h>
#include "ctimer.h"

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

    struct timespec tp;
    {
        t.Begin();
        for (int i=0; i<MAX_COUNT; ++i)
        {//clock_gettime(CLOCK_MONOTONIC, &tp);;
            clock_gettime(CLOCK_REALTIME, &tp);;
        }

        printf("clock_gettime():\n");
        printf("elapse : %5.5f sec\n", t.GetElapseTimeSec());
        printf("\n");

    }

    return 0;
}
