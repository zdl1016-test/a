#include <stdio.h>
#include <string.h>

// lodsb 从si指向的内存读取一个字节存入累加器al, 然后si根据方向自动+1或-1
// scasb 计算AL寄存器值和EDI指向内存的值的差值 al-(%edi)x寄存器值复制到edi指向的内存单元
// 从累加器al读取值存入di指向的内存. 然后di根据方向自动+1或-1
int main(int argc, char* argv[])
{
    char* p = "abcd";
    char buf[128] = {0};
    int count = strlen(p);
    int i = 0;
    

    __asm__ __volatile__(
            "cld;\t\n"
            "1:decl %0;\t\n"
            "js 2;\t\n"
            "lodsb;\t\n"
            "stosb;\t\n"
            "lodsb;\t\n"
            "stosb;\t\n"
            "lodsb;\t\n"
            "stosb;\t\n"
            /*"jmp 1;\t\n"*/
            "2:\t\n"
            :
            :"c"(0), "S"(p), "D"(buf)
            );
    /*__asm__ __volatile__(*/
            /*"cld;\t\n"*/
            /*"1:decl %0;\t\n"*/
            /*"js 2f;\t\n"*/
            /*"lodsb;\t\n"*/
            /*"scasb;\t\n"*/
            /*"jmp 1f;\t\n"*/
            /*"2:\t\n"*/
            /*:"=c"(3)*/
            /*:"S"(p), "D"(buf)*/
            /*);*/

    printf ("buf:[%s]\n", buf);
    return 0;
}
