#include <stdio.h>
#include <stdlib.h>
#include "../time/ctimer.h"

int main()
{
    int cache_line  = 64;
    int cache_size  = 1024*1024*4;  
    //int cache_size  = 1024;  

    int buf_size = cache_size * 12;
    char * buf = (char*)malloc(buf_size);
    memset(buf, buf_size, 0);

    CMyTimer t1;

    int tmp = 0;
    int count = 1000000;

    int pos = 0;
    t1.Begin();
    //for (int i=0; i<count; ++i)
    //{
        //for (int j=0;j<12;j++)
        //{
            //pos = j*cache_size;
            //for (int k=0; k<10; k+=4)
            //{
            //tmp = *(int*)&buf[pos+k];
            //tmp++;
            //*(int*)&buf[pos+k] = tmp;
            //}
        //}
    //}
    for (int i=0; i<count; ++i)
    {
            pos = 0*cache_size;
            for (int k=0; k<10; k+=4)
            {
            tmp = *(int*)&buf[pos+k];
            tmp++;
            *(int*)&buf[pos+k] = tmp;
            }
            pos = 1*cache_size;
            for (int k=0; k<10; k+=4)
            {
            tmp = *(int*)&buf[pos+k];
            tmp++;
            *(int*)&buf[pos+k] = tmp;
            }
            pos = 2*cache_size;
            for (int k=0; k<10; k+=4)
            {
            tmp = *(int*)&buf[pos+k];
            tmp++;
            *(int*)&buf[pos+k] = tmp;
            }
            pos = 3*cache_size;
            for (int k=0; k<64; k+=4)
            {
            tmp = *(int*)&buf[pos+k];
            tmp++;
            *(int*)&buf[pos+k] = tmp;
            }
    }
    printf("use time : %5.1f\n", t1.GetElapseTimeMS());

    t1.Begin();
    //for (int i=0; i<count; ++i)
    //{
            //pos = 0*cache_size;
            //for (int k=0; k<10; k+=4)
            //{
            //tmp = *(int*)&buf[pos+k];
            //tmp++;
            //*(int*)&buf[pos+k] = tmp;
            //}
            //pos = 1*cache_size;
            //for (int k=0; k<10; k+=4)
            //{
            //tmp = *(int*)&buf[pos+k];
            //tmp++;
            //*(int*)&buf[pos+k] = tmp;
            //}
            //pos = 2*cache_size;
            //for (int k=0; k<10; k+=4)
            //{
            //tmp = *(int*)&buf[pos+k];
            //tmp++;
            //*(int*)&buf[pos+k] = tmp;
            //}
            //pos = 3*cache_size;
            //for (int k=0; k<10; k+=4)
            //{
            //tmp = *(int*)&buf[pos+k];
            //tmp++;
            //*(int*)&buf[pos+k] = tmp;
            //}
    //}
    for (int j=0;j<4;j++)
    {
            pos = j*cache_size;
        for (int i=0; i<count; ++i)
        {
            //tmp = *(int*)&buf[0];
            //tmp++;
            //*(int*)&buf[0] = tmp;

            //tmp = *(int*)&buf[0];
            //tmp++;
            //*(int*)&buf[0] = tmp;
            for (int k=0; k<64; k+=4)
            {
            tmp = *(int*)&buf[pos+k];
            tmp++;
            *(int*)&buf[pos+k] = tmp;
            }
        }
    }
    printf("use time : %5.1f\n", t1.GetElapseTimeMS());
    free(buf);
}
