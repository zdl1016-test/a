#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <ext/hash_map>
#include <list>
#include <unistd.h> // for usleep

using namespace std;
using namespace __gnu_cxx;


#ifndef foreach
#define foreach(container,it) \
            for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

int main()
{
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);

    foreach (l, it)
    {
        printf("%d\n", *it);
    }
    printf("------------------------\n");

    // move the first element to end
    l.splice(l.end(), l, l.begin());

    foreach (l, it)
    {
        printf("%d\n", *it);
    }
    printf("------------------------\n");

    l.clear();
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);

    // move the last two element to begin 
    l.splice(l.begin(), l, --(--l.end()), l.end());

    foreach (l, it)
    {
        printf("%d\n", *it);
    }
    printf("------------------------\n");
    return 0;
}
