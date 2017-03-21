#include <stdio.h>

class base
{
    public:
        virtual void send() = 0;
};

class child : public base
{
    void send ();
};

void child::send()
{
    printf ("send in child\n");
}
int main()
{
    base * pb = new child();
    //base * pb = new base();
    base * pb2 = pb;
    pb->send();
    delete pb;
    pb2 = 0;
    pb2->send();

    return 0;
}
