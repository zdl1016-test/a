#include <stdio.h>

#define MAX_SIZE_T           (~(size_t)0)

/* The byte and bit size of a size_t */
#define SIZE_T_SIZE         (sizeof(size_t))
#define SIZE_T_BITSIZE      (sizeof(size_t) << 3)

/* Some constants coerced to size_t */
/* Annoying but necessary to avoid errors on some platforms */
#define SIZE_T_ZERO         ((size_t)0)
#define SIZE_T_ONE          ((size_t)1)
#define SIZE_T_TWO          ((size_t)2)
#define SIZE_T_FOUR         ((size_t)4)
#define TWO_SIZE_T_SIZES    (SIZE_T_SIZE<<1)
#define FOUR_SIZE_T_SIZES   (SIZE_T_SIZE<<2)
#define SIX_SIZE_T_SIZES    (FOUR_SIZE_T_SIZES+TWO_SIZE_T_SIZES)
#define HALF_MAX_SIZE_T     (MAX_SIZE_T / 2U)

#define MALLOC_ALIGNMENT    (8u)

#define CHUNK_ALIGN_MASK    (MALLOC_ALIGNMENT - SIZE_T_ONE)

#define align_offset(A)\
 ((((size_t)(A) & CHUNK_ALIGN_MASK) == 0)? 0 :\
    ((MALLOC_ALIGNMENT - ((size_t)(A) & CHUNK_ALIGN_MASK)) & CHUNK_ALIGN_MASK))

#define is_page_aligned(S)\
   (((size_t)(S) & (mparams.page_size - SIZE_T_ONE)) == 0)


int main()
{
    printf("SIZE_T_SIZE     : %d\n", SIZE_T_SIZE);
    printf("SIZE_T_BITSIZE  : %d\n", SIZE_T_BITSIZE);
    printf("SIZE_T_ZERO     : %d\n", SIZE_T_ZERO);
    printf("SIZE_T_ONE      : %d\n", SIZE_T_ONE);
    printf("SIZE_T_TWO      : %d\n", SIZE_T_TWO);
    printf("SIZE_T_FOUR     : %d\n", SIZE_T_FOUR);

    printf("TWO_SIZE_T_SIZES    : %d\n", TWO_SIZE_T_SIZES);
    printf("FOUR_SIZE_T_SIZES   : %d\n", FOUR_SIZE_T_SIZES);
    printf("SIX_SIZE_T_SIZES    : %d\n", SIX_SIZE_T_SIZES);
    printf("HALF_MAX_SIZE_T     : %d\n", HALF_MAX_SIZE_T);

    printf("align_offset(1) : %d\n", align_offset(1));
    printf("align_offset(4) : %d\n", align_offset(4));
    printf("align_offset(8) : %d\n", align_offset(8));

}
