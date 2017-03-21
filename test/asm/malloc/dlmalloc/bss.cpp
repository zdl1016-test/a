#include <stdio.h>

int a1;
int a2;

struct CA
{
    int a1;
    int a2;
    CA(){a1 = 1;}
};
CA ca;
int main()
{
    int a3;
    int a4;

    // a1, a2 in bss be init to zero
    // a3, a3 in stack is random
    printf("%d, %d, %d, %d\n", a1, a2, a3, a4);

    CA ca2;
    // ca.a2 in bss be init to zero
    // ca2.a2 in stack is random
    printf("%d, %d, %d, %d\n", ca.a1, ca.a2, ca2.a1, ca2.a2);
    return 0;
}
