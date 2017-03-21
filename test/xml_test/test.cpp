#include <stdio.h>
#include <string>
//#include <cstring>

std::string Trim(std::string str) 

{
    if(str.empty()) 
    { 
        return str; 
    }
    
    str.erase(0,str.find_first_not_of(" ")); 
    str.erase(str.find_last_not_of(" ")+ 1); 

    return str; 

}
using namespace std;
int main()
{
    string str = " 800 ";
    int ival = atoi(str.c_str());
    printf( "ival = %d\n", ival );
    unsigned int uival = strtoul(str.c_str(), 0, 10);
    printf( "uival = %u\n", uival);

    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");
    
    str = "  hello world.cgi  ";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());

    printf("\n");
    str = "  hello world.cgi中国  ";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");

    str = "没过  hello world.cgi  ";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");

    str = "没过  hello world.cgi  ";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");


    str = "没过  hello world.cgi";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");


    str = "            没过  hello world.cgi s啦几i    ";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");

    str = "";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");
    
    str = "      ";
    printf("org str   : [%s]\n", str.c_str());
    printf("after trim: [%s]\n", Trim(str).c_str());
    printf("\n");


   
 

	return 0;
}


