#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

void Buf2HexStr(const string &src, string &hex_str)
{
    hex_str.resize(src.length()*2);
    char tmpbuf[3] = {0};
    for (unsigned i=0; i<src.length(); ++i)
    {
        snprintf(tmpbuf, sizeof(tmpbuf), "%02X\n", (unsigned char)src[i]);
        hex_str[0+i*2] = tmpbuf[0];
        hex_str[1+i*2] = tmpbuf[1];
    }
}

void HexStr2Buf(const string &src, string &dec_str)
{
    if (src.length() < 2)
        return;

    dec_str.resize(src.length()/2);

    char tmpbuf[3] = {0};
    for (unsigned i=0; i<dec_str.length(); ++i)
    {
        tmpbuf[0] = src[i*2];
        tmpbuf[1] = src[i*2+1];
        tmpbuf[2] = 0;
        dec_str[i] = (unsigned char)strtoul(tmpbuf, 0, 16);
    }
}

int main(int argc, char* argv[])
{
    
    unsigned char arr[] = {0,1,2,3,4,5,4,3,2,1,-1,-2,67,255,254};
    string src;
    src.assign(arr, arr+sizeof(arr));
    string hex_str;
    Buf2HexStr(src, hex_str);
    printf("hex_str:[%s]\n", hex_str.c_str());


    string dec_str;
    HexStr2Buf(hex_str, dec_str);
    printf("dec_str:[%s]\n", dec_str.c_str());

    if (dec_str == src)
    {
        printf("equal");
    }


    printf("\n");

    for (unsigned int i=0; i<dec_str.length(); ++i)
    {
        printf("%i:%d\n", i, (unsigned char)dec_str[i]);
    }

    return 0;
}

