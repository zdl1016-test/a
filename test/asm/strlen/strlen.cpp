#include <stdio.h>
#include <string.h>

int my_strlen(const char * str)
{
    const char * pstr = str;
    
    // 先将地址边界对齐  & 操作更高效, 避免了%取余运算
    //for (; (unsigned long)pstr%(sizeof(long)) != 0; ++pstr)
    for (; (unsigned long)pstr & (sizeof(long)-1) != 0; ++pstr)
    {
        if (pstr[0] == '\0')
            return pstr - str;
    }

    unsigned long himagic = 0x80808080;
    unsigned long lomagic = 0x01010101;

    // 如果是64位
    if (sizeof(long) > 4)
    {
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }

    unsigned long longword = 0;
    for (;; pstr+=sizeof(long))
    {
        longword = *(unsigned long*)pstr;

        // (longword - lomagic) & ~longword 可测试出, 相减之后longword变量的那些bit发生了改变
        // ... & himagic 可测试出, 变化的那些bit位中是否包含各个字节的最高位
        if (((longword - lomagic) & ~longword & himagic) != 0)
        {
            if (pstr[0] == '\0')
                return pstr - str;
            if (pstr[1] == '\0')
                return pstr - str + 1;
            if (pstr[2] == '\0')
                return pstr - str + 2;
            if (pstr[3] == '\0')
                return pstr - str + 3;
            if (sizeof(long) > 4)
            {
                if (pstr[4] == '\0')
                    return pstr - str + 4;
                if (pstr[5] == '\0')
                    return pstr - str + 5;
                if (pstr[6] == '\0')
                    return pstr - str + 6;
                if (pstr[7] == '\0')
                    return pstr - str + 7;
            }
        }
    }

    // never come to here...
    return 0;
}
int main()
{
    char * str = "helloworldaaaaa";
    int ret = 0;

    ret = my_strlen(str+1);
    printf("len : %d\n", ret);

    ret = strlen(str+1);
    printf("len : %d\n", ret);

    return 0;
    
}
