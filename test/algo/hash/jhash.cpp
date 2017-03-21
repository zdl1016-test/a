#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef u32
#define u32 unsigned int
#endif

#include "/home/nemo/code/linux-2.6.11.11/include/linux/jhash.h"


int main()
{
    // 假设hash table的长度为8(2^3)
    int bits = 3;
    int elem_num = 1<<bits;
    int vec[elem_num];

    for (int i=0; i<elem_num; ++i)
        vec[i] = 0;

    srand(time(NULL));

    for(int i=0; i<10; i++)
    {
        int num = rand();
        //printf("num:%d\n", num);
        int elem_idx = jhash(&num, sizeof(num), bits);
        printf("elem_idx:%d\n", elem_idx);
        //vec[elem_idx] ++;
    }

    //printf("========================\n");
    //for (int i=0; i<elem_num; ++i)
    //{
        //printf("vec[%d]: %d\n", i, vec[i]);
    //}

    return 0;
}
