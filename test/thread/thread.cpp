#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* Prints x’s to stderr. The parameter is unused. Does not return. */
void* print_xs (void* arg)
{
    printf("in thread %u, arg is : %d\n", pthread_self(), *(int*)arg);

    pthread_detach(pthread_self());
    return NULL;
}

class CA
{
public:
        CA():i(55){}

public:
    void StartThread()
    {
        pthread_t thread_id;
        pthread_create (&thread_id, NULL, CA::ThreadFun, this);
    }

    static void* ThreadFun(void *arg)
    {
        CA *obj = (CA*)arg;
        printf("in thread %u, arg is : %d\n", pthread_self(), obj->i);
        pthread_detach(pthread_self());
    }

public:
    int i;
};

/* The main program. */
int main ()
{
    pthread_t thread_id;

    int num1 = 11;
    int num2 = 22;
    int num3 = 33;

    pthread_create (&thread_id, NULL, &print_xs, &num1);
    printf("in main thread , child thread : %u\n", thread_id);

    pthread_create (&thread_id, NULL, &print_xs, &num2);
    printf("in main thread , child thread : %u\n", thread_id);

    CA obj;
    obj.StartThread();

    /* Print o’s continuously to stderr. */
    while (1)
    {
        printf("in main thread\n");
        sleep(1);
    }
    return 0;
}
