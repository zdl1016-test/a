int main()
{
    __asm__(
            "movl $1, %eax;"
            "movl $-1, %ebx;" /*返回值 -1*/
            "int $0x80"
            );
}
