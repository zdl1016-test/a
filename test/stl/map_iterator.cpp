#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <unistd.h> // for usleep

using namespace std;


#ifndef foreach
#define foreach(container,it) \
            for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif
int main()
{
    map<int , int> im;

    im[1] = 1;
    im[10] = 10;
    im[20] = 20;
    im[21] = 21;

    map<int, int>::iterator it;
    map<int, int>::iterator it1;
    map<int, int>::iterator it2;
    map<int, int>::iterator it3;

    it1 = im.find(1);
    it2 = im.find(10);
    it3 = im.find(20);

    //--------------------------------
    // 边遍历便插入测试
    //--------------------------------
    int i = 30;
    foreach(im,it)
    {
        printf("it->first:%d, it->second:%d\n", it->first, it->second);
        if (i<40)
        {
            im[++i] = i;
            im[-i] = -i;
        }
    }

    printf("-----------------\n");

    //--------------------------------
    // 遍历完成后,拿着以前的iterator测试
    //--------------------------------
    for(it=it3; it!=im.end(); ++it)
    {
        printf("it->first:%d, it->second:%d\n", it->first, it->second);
    }
    return 0;
}
