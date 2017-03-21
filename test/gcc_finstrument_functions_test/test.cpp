#include <stdio.h>

#define MAX_NUM 1000
int g_count = 10;
void fun_1()
{
    int i=0;
    int g=0;
    for(; i<MAX_NUM; ++i)
    {
        g += i*i;
    }
}

void fun_2()
{
    int i=0;
    int g=0;
    for(; i<MAX_NUM*4; ++i)
    {
        g += i*i;
    }
}

void fun_3()
{
    if (g_count < 0)
    {
        return;
    }

    g_count--;

    fun_1();
    fun_2();
    fun_3();
}
void (*fun)() ;

class CA1
{
    public:
    void fun_ca1()
    {
        printf("xxx");
    }
};
class CA2
{
    public:
    CA2()
    {
        printf("in ca2");
    }

    void fun_ca2()
    {
       m_ca1.fun_ca1(); 
    }
    CA1 m_ca1;
};

int main()
{
    printf("hello world\n");

    fun_2();
    fun_1();

    CA1 ca;
    ca.fun_ca1();
    CA2 ca2;
    ca2.fun_ca2();
    

    return 0;
}
