#include <stdio.h>
#include <stdlib.h>
#include "ctimer.h"

int main()
{
    char buf[512];
    printf("buf[0]'s addr [%x][%d]\n", (int)(char*)&buf[0],(int)(char*)&buf[0]);
    printf("buf[1]'s addr [%x][%d]\n", (int)(char*)&buf[1],(int)(char*)&buf[1]);

    CMyTimer t1;

    int tmp = 0;
    int count = 1000000;

    memset(buf, sizeof(buf), 0);

    t1.Begin();
    for (int i=0; i<count; ++i)
    {
        for (int j=1;j<(sizeof(buf)-4);j+=4)
        {
            tmp = *(int*)&buf[j];
            tmp++;
            *(int*)&buf[j] = tmp;
        }
    }
    printf("use time : %5.1f\n", t1.GetElapseTimeMS());

    memset(buf, sizeof(buf), 0);
    t1.Begin();
    for (int i=0; i<count; ++i)
    {
        for (int j=2;j<(sizeof(buf)-4);j+=4)
        {
            tmp = *(int*)&buf[j];
            tmp++;
            *(int*)&buf[j] = tmp;
        }
    }
    printf("use time : %5.1f\n", t1.GetElapseTimeMS());

    memset(buf, sizeof(buf), 0);
    t1.Begin();
    for (int i=0; i<count; ++i)
    {
        for (int j=3;j<(sizeof(buf)-4);j+=4)
        {
            tmp = *(int*)&buf[j];
            tmp++;
            *(int*)&buf[j] = tmp;
        }
    }
    printf("use time : %5.1f\n", t1.GetElapseTimeMS());

    memset(buf, sizeof(buf), 0);
    t1.Begin();
    for (int i=0; i<count; ++i)
    {
        for (int j=0;j<(sizeof(buf)-4);j+=4)
        {
            tmp = *(int*)&buf[j];
            tmp++;
            *(int*)&buf[j] = tmp;
        }
    }
    printf("use time : %5.1f\n", t1.GetElapseTimeMS());
    return 0;
}