#include <stdio.h>


static inline unsigned long __xchg(unsigned long x, volatile void * ptr, int size) 
{
    switch (size) {           
        case 1:
            __asm__ __volatile__("xchgb %b0,%1"
                :"=q" (x)     
                :"m" (*((int*)ptr)), "0" (x)     
                :"memory");   
            break;            
        case 2:               
            __asm__ __volatile__("xchgw %w0,%1"
                :"=r" (x)     
                :"m" (*((int*)ptr)), "0" (x)     
                :"memory");
            break;            
        case 4:               
            __asm__ __volatile__("xchgl %0,%1"
                :"=r" (x)     
                :"m" (*((int*)ptr)), "0" (x)     
                :"memory");   
            break;            
    }  
    return x;
}

int main()
{
    int a = 1;
    int b = 2;
    int c = 0;
    printf("a:%d, b:%d, c:%d\n", a, b, c);
    c = __xchg(a, &b, 4);
    printf("a:%d, b:%d, c:%d\n", a, b, c);
    return 0;

}
