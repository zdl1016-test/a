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
    struct in_addr ip;
    if (argc < 2)
    {
        printf("usage: %s ipint(a number)\n", argv[0]);
        return 0; 
    }

    //-------------------------------------------
    
    ip.s_addr = strtoul(argv[1], 0, 10);

    // 先从主机2net, inet_ntoa的输入参数要求是net需
    ip.s_addr = htonl(ip.s_addr);

    // 非线程安全
    //char* ipstr = 0;
    //ipstr = inet_ntoa(ip);

    char ipsz[16] = {0};
    const char* ipstr = inet_ntop(AF_INET, (void*)&ip, (char*)&ipsz[0], sizeof(ipsz));

  
    printf("treat as host byte order : \n");
    if (ipstr)
    {
        printf("%s\n", ipstr);
    }
    else
    {
        printf("convert failed.\n");
    }

    //-------------------------------------------

    printf("\n");
    printf("treat as net byte order : \n");
    ip.s_addr = strtoul(argv[1], 0, 10);
    ipstr = inet_ntoa(ip);
  
    if (ipstr)
    {
        printf("%s\n", ipstr);
    }
    else
    {
        printf("convert failed.\n");
    }

    return 0;

}
