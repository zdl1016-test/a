#include <stdio.h>

typedef unsigned int bindex_t; 

// have bugs, only valid when have one 1bit in X
//#define compute_bit2idx(X, I)\
//{\
      //unsigned int Y = X - 1;\
      //unsigned int K = Y >> (16-4) & 16;\
      //unsigned int N = K;        Y >>= K;\
      //N += K = Y >> (8-3) &  8;  Y >>= K;\
      //N += K = Y >> (4-2) &  4;  Y >>= K;\
      //N += K = Y >> (2-1) &  2;  Y >>= K;\
      //N += K = Y >> (1-0) &  1;  Y >>= K;\
      //I = (bindex_t)(N + Y);\
//}

#define compute_bit2idx(X, I)\
{\
      unsigned int J;\
      J = __builtin_ctz(X); \
      I = (bindex_t)J;\
}

int main()
{
    int len = 0;
    unsigned int num = 1;
    int X = 1;
    int I = 0;
    for (int i=0; i<32; i++)
    {
        compute_bit2idx(X, I);
        printf("x:%d, i:%d\n", X, I);
        X <<= 1;
    }

    X = 3;
    compute_bit2idx(X, I);
    printf("x:%d, i:%d\n", X, I);
    X = 7;
    compute_bit2idx(X, I);
    printf("x:%d, i:%d\n", X, I);
}
