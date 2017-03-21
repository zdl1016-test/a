#include "markupstl.h"
#include "Markup.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
    CMarkupSTL xml;
    //CMarkup xml;
    if (!xml.Load("./api_config.xml"))
    {
        printf("load file fail.\n");
        return -1;
    }

    if (!xml.FindElem("APP_CONFIG"))
    {
        printf("find APP_CONFIG fail.\n");
        return -1;
        
    }
    xml.IntoElem();

    if (!xml.FindElem("api_list"))
    {
        printf("find api_list fail.\n");
        return -1;
    }
    xml.IntoElem();

    if (!xml.FindElem("qos"))
    {
        printf("find api_list fail.\n");
        return -1;
    }
    //xml.IntoElem();
    printf("rebuild_internal: [%s] rebuild_max: [%s]", 
        xml.GetAttrib("rebuild_internal").c_str(),
        xml.GetAttrib("rebuild_max").c_str()
        );
    //cout<<""
    
    printf("\nhello world\n");
    return 0;
}
