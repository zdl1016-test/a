#include </usr/include/unistd.h>
#include <stdio.h>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <arpa/inet.h>
using namespace std;

#include <time.h>
#include <arpa/inet.h>

#ifndef foreach
#define foreach(container,it) \
        for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

int main(int argc, char* argv[])
{
    int ret = 0;
    string ip_str;
    struct in_addr ip;

    if (argc < 2)
    {
        printf("usage: %s ipstr\n", argv[0]);
        return 0; 
    }

    // 1)
    // 更简单的函数 inet_addr, 如果返回-1, 则认为出错
    // 2) in_addr 结构体可以直接转为int型使用
    //ip_str = argv[1];
    ip_str = "";
    ret = inet_aton(ip_str.c_str(), &ip);
    if (0 == ret)
    {
        printf("failed. ret:%d\n", ret);
        return -1;                                                                                                                                                           
    }
    printf("dec:%u, hex:%X (net byte order)\n", ip.s_addr, ip.s_addr);
 
    unsigned int t = ntohl(ip.s_addr);
    printf("dec:%u, hex:%X (host byte order)\n", t, t);

    return 0;

}
