#include <sstream>
#include <stdio.h>
using namespace std;
int main()
{
    stringstream ss;
    string result;
    int n=1;
    ss.clear(); 
    ss<<n;
    ss>>result;
    printf("result : %s, str : %s\n", result.c_str(), ss.str().c_str());

    n=2;
    ss.clear();
    ss<<n;
    ss>>result;
    printf("result : %s, str : %s\n", result.c_str(), ss.str().c_str());

    n=3;
    ss.str("");
    ss<<n;
    ss>>result;
    printf("result : %s, str : %s\n", result.c_str(), ss.str().c_str());

    n=4;
    ss.clear();
    ss.str("");
    ss<<n;
    ss>>result;
    printf("result : %s, str : %s\n", result.c_str(), ss.str().c_str());
}
