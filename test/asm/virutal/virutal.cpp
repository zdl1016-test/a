#include <stdio.h>

class CA
{
public:
    int i;
    virtual void fun(){}
    virtual void fun2(){}
    void fun3() {fun(); fun2();}
};

class CB : public CA
{
public:
    virtual void fun(){}
};

int main()
{
    CA a;
    printf("%d\n", sizeof(a));
    a.fun();
    a.fun2();

    CB b;
    b.fun3();
    return 0;

}
