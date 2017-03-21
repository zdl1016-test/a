#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int func(int n)
{
    int vec[10];
    int sum=0;
    int i;

    *((int*)(0))=0;

    vec[0] = 0;
    vec[1] = 1;
    vec[2] = 2;
    vec[3] = 3;
    vec[4] = 0;
    vec[5] = 5;


    char *vec2 = (char*)malloc(10*sizeof(char));
    memset(vec2, 0, sizeof(char)*10);

    vec2[0] = 'a';
    vec2[1] = 'b';
    vec2[2] = 2;
    vec2[3] = 'd';
    vec2[4] = 0;
    vec2[5] = 5;



    for(i=0; i<n; i++)
    {
        sum += i;
    }

    return sum;
}

int main()
{
    int i;
    long result = 0;
    for(i=1; i<=100; i++)
    {
        result += i;
    }

    result = func(10);

    printf("result[1-100] = %d\n", result);
    printf("result[1-250] = %d\n", func(250));

    return 11;
}
