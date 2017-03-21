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

int main()
{
    
	unsigned int t = 0x01020304;
    printf("org int: d:[%d], x:[%x] \n", t, t);

    printf("host byte order : \n");
	printf("t[0]:%x\n", ((char*)&t)[0]);
	printf("t[1]:%x\n", ((char*)&t)[1]);
	printf("t[2]:%x\n", ((char*)&t)[2]);
	printf("t[3]:%x\n", ((char*)&t)[3]);
	
    unsigned int t2 = htonl(t);

    printf("net byte order : \n");
	printf("t[0]:%x\n", ((char*)&t2)[0]);
	printf("t[1]:%x\n", ((char*)&t2)[1]);
	printf("t[2]:%x\n", ((char*)&t2)[2]);
	printf("t[3]:%x\n", ((char*)&t2)[3]);

    printf("\n");

    if (0x4 == (((char*)&t)[0]) )
    {
        printf("your host is small end\n");
    }
    else
    {
        printf("your host is big end\n");
    }

    printf("PS: net order is big end\n");
   
    return 0;

}
