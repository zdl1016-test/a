#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long hash_long(unsigned long val, unsigned int bits) 
{ 
    unsigned long hash = val * 0x9e370001UL; 
    return hash >> (32 - bits); 
}

int main()
{
    // 假设hash table的长度为8(2^3)
    int bits = 3;
    int elem_num = 1<<bits;
    int vec[elem_num];

    for (int i=0; i<elem_num; ++i)
        vec[i] = 0;

    srand(time(NULL));

    for(int i=0; i<1000; i++)
    {
        int num = rand();
        //printf("num:%d\n", num);
        int elem_idx = hash_long(num, bits);
        vec[elem_idx] ++;
    }

    printf("========================\n");
    for (int i=0; i<elem_num; ++i)
    {
        printf("vec[%d]: %d\n", i, vec[i]);
    }

    return 0;
}
