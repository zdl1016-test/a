#include <stdio.h>

static inline int find_first_zero_bit(const unsigned long *addr, unsigned size);

/**
 * find_next_zero_bit - find the first zero bit in a memory region
 * @addr: The address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The maximum size to search
 */
int find_next_zero_bit(const unsigned long *addr, int size, int offset)
{
    unsigned long * p = ((unsigned long *) addr) + (offset >> 5);
    int set = 0, bit = offset & 31, res;

    if (bit) {
        /*
         * Look for zero in the first 32 bits.
         */
        __asm__("bsfl %1,%0\n\t"
            "jne 1f\n\t"
            "movl $32, %0\n"
            "1:"
            : "=r" (set)
            : "r" (~(*p >> bit)));
        if (set < (32 - bit))
            return set + offset;                                                                                                
        set = 32 - bit;
        p++;
    }
    /*
     * No zero yet, search remaining full bytes for a zero
     */
    res = find_first_zero_bit (p, size - 32 * (p - (unsigned long *) addr));
    return (offset + set + res);
}


/**
 * find_first_zero_bit - find the first zero bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum size to search
 *
 * Returns the bit-number of the first zero bit, not the number of the byte
 * containing a bit.
 */
static inline int find_first_zero_bit(const unsigned long *addr, unsigned size)
{
    int d0, d1, d2;
    int res;

    if (!size)
        return 0;
    /* This looks at memory. Mark it volatile to tell gcc not to move it around */
    __asm__ __volatile__(
        "movl $-1,%%eax\n\t"
        "xorl %%edx,%%edx\n\t"
        "repe; scasl\n\t"
        "je 1f\n\t"
        "xorl -4(%%edi),%%eax\n\t"
        "subl $4,%%edi\n\t"
        "bsfl %%eax,%%edx\n"
        "1:\tsubl %%ebx,%%edi\n\t"
        "shll $3,%%edi\n\t"
        "addl %%edi,%%edx"
        :"=d" (res), "=&c" (d0), "=&D" (d1), "=&a" (d2)
        :"1" ((size + 31) >> 5), "2" (addr), "b" (addr) : "memory");
    return res;
}

int main()
{
    unsigned long buf[128] = {0};
    buf[0] = 0xFFFFFFFF;
    buf[1] = 0x7FFFFFFF;
    //buf[1] = 0x0;
    int ret = find_next_zero_bit(buf, sizeof(buf), 0); 
    printf("first zero bit : %d\n", ret);
    return 0;

}
