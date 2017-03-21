#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<signal.h>

int cpu_task();
int foo2();
void foo_sleep1()
{
    sleep(1);
}
int foo_empty(int i)
{
    return i+1;
}

int foo1()
{
    cpu_task();
    return 0;
}

int foo2()
{
    for (int i=0; i<1000; ++i)
    {
        foo1();
    }
    return 0;
}

int cpu_task()
{
    for (int i=0; i<50000; ++i)
    {
        char* p = (char*)malloc(1);
        free(p);
    }
    return 0;
}

static void sighandler( int sig_no )   
{   
    printf("recv sig : %d\n", sig_no);
    exit(0);   
}   


int main()
{
    signal( SIGUSR1, sighandler);
    printf("hello world\n");

    int count = 0;
    for (int i=0; i<50; ++i)
    {
        foo_empty(count);
    }

    foo1();
    foo1();
    foo1();

    foo2();

    for (int i=0; i<50; ++i)
    {
        foo_sleep1();
        printf("sleep .. \n");
        foo_empty(count);
    }

    return 0;
}
