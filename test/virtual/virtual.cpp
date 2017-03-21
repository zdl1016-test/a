#include <stdio.h>

class CA
{
public:
    virtual ~CA(){}
    virtual void foo()
    {
        printf("CA::foo\n");
    }
};

class CB : public CA
{
public:
    virtual ~CB(){}
    virtual void foob()
    {
        printf("CB::foo\n");
    }
};

class CC : public CA
{
public:
    virtual ~CC(){}
    virtual void fooc()
    {
        printf("CC::foo\n");
    }
};

class CD : public CC, public CB
{
public:
    virtual ~CD(){}
    virtual void food()
    {
        printf("CD::foo, %u, %u\n", this, (int*)this+1);
        printf("CD::foo, %u, %u\n", this, *(int*)this);
        printf("%d", &CD::food);
        //void (*ptr)() ;
        //ptr = (void (*)()) ((int*)(*(int*)this) + 1);
        //ptr();

        //((void*)this + 1)();
    }
};

int main()
{
    CD d;
    d.food();
    return 0;

}
