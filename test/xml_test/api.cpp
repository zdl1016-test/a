#include <stdio.h>
#include <string>
#include "Markup.h"

using namespace std;

CMarkup g_xml_gen;

int LoadAppList_App_ARole(CMarkup &xml)
{
    if (!xml.FindElem("id"))
        return -1;

    //xml.GetData()
    printf("id : %s ", xml.GetData().c_str());
    return 0;
}

int LoadAppList_AApp(CMarkup &xml)
{
    int iRet = 0;
    (void)iRet;
    
    if (!(xml.FindElem("id") &&
        xml.FindElem("name") &&
        xml.FindElem("desc") &&
        xml.FindElem("appkey") &&
        xml.FindElem("role_list") &&
        xml.FindElem("default_max_load") &&
        xml.FindElem("api_list")
        ))
    {
        printf("find APP_CONFIG->app_list->app->[id, name, desc, appkey, "
                "ip_list, role_list, default_max_load, api_list] failed.\n");
        return -1;            
    }

    // 前面调用过FindElem了,现在使其复位
    xml.ResetMainPos();

    xml.FindElem("id");
    printf("id : %s \n", xml.GetData().c_str());
    
    xml.FindElem("name");
    printf("name : %s \n", xml.GetData().c_str());

    xml.FindElem("desc");
    printf("desc : %s \n", xml.GetData().c_str());

    xml.FindElem("appkey");
    printf("appkey : %s \n", xml.GetData().c_str());
    
    // parse role_list
    xml.FindElem("role_list");
    xml.IntoElem();
    while (xml.FindElem("role"))
    {
        printf("role id:%s ", xml.GetAttrib("id").c_str());
#if 0
        xml.IntoElem();
        iRet = LoadAppList_App_ARole(xml);
        if (0 != iRet)
        {
            return -1;
        }
        xml.OutOfElem();
#endif        
    }
    xml.OutOfElem();// out of role_list
    printf("\n");


    xml.FindElem("default_max_load");
    printf("default_max_load : %s \n", xml.GetData().c_str());

    // parse ip_list
    xml.FindElem("api_list");
    int iNumApi = 0;
    xml.IntoElem();
    while (xml.FindElem("api"))
    {
        ++iNumApi;
        xml.IntoElem();
        if (!xml.FindElem("name"))
            return -1;

        printf("name:%s\n", xml.GetData().c_str());
        
        if (!xml.FindElem("max_load"))
            return -1;

        printf("max_load:%s\n", xml.GetData().c_str());            

        xml.OutOfElem();
    }    
    xml.OutOfElem();// out of ip_list
    
    // 允许IP的个数为0
    printf("api num: %d\n", iNumApi);
    printf("\n");
    
    return 0;
}

int LoadAppList(CMarkup &xml)
{
    int iRet = 0;
    // 复位整个文档
    xml.ResetPos();

    if (!xml.FindElem("APP_CONFIG"))
    {
        return -1;
    }

    xml.IntoElem();
    if (!xml.FindElem("app_list"))
    {
        return -1;
    }

    xml.IntoElem();
    int iNumApp = 0;
    while (xml.FindElem("app"))
    {
        printf("app idx: %d\n", iNumApp);
        ++iNumApp;
        
        xml.IntoElem();
        //xml.FindElem("id");
        //printf("id: %s", xml.GetData().c_str());        
        iRet = LoadAppList_AApp(xml);
        if (0 != iRet)
        {
            printf("LoadAppList_AApp(xml) failed.\n");
            return -1;
        }
        xml.OutOfElem(); // outof ip_list               
        
        printf("\n");
    }
    xml.OutOfElem();// outof app

    if (iNumApp <= 0)
    {
        printf("app num: 0\n");
        return -1;
    }

    return 0;
}

int LoadApiList_Api_AServer(CMarkup &xml)
{
    if (!xml.FindElem("ip"))
    {
        printf("find APP_CONFIG->api_list->api->server_list"
                "->server->[ip] failed.\n");
        return -1;                    
    }
    printf("ip: %s\n", xml.GetData().c_str());

    if (!xml.FindElem("weight"))
    {
        printf("find APP_CONFIG->api_list->api->server_list"
                "->server->[weight] failed.\n");
        return -1;                     
    }
    printf("weight: %s\n", xml.GetData().c_str());  

    if (!xml.FindElem("max_queue_req"))
    {
        printf("find APP_CONFIG->api_list->api->server_list"
                "->server->[max_queue_req] failed.\n");
        return -1;                       
    }
    printf("max_queue_req: %s\n", xml.GetData().c_str());    

    return 0;
}

int LoadApiList_AApi(CMarkup &xml)
{
    int iRet = 0;
    (void)iRet;
        
    if (!(xml.FindElem("id") &&
        xml.FindElem("name") &&
        xml.FindElem("desc") &&
        xml.FindElem("server_list"))
        )
    {
        printf("find APP_CONFIG->api_list->api->[id, name, desc, "
                "server_list] failed.\n");
        return -1;
    }

    xml.ResetMainPos();
    
    xml.FindElem("id");
    printf("id: %s, ", xml.GetData().c_str());    

    xml.FindElem("name");
    printf("name: %s ", xml.GetData().c_str());
    
    xml.FindElem("desc");
    printf("desc: %s \n", xml.GetData().c_str());

    xml.FindElem("server_list");
    xml.IntoElem();
    { 
        int iNumSvr = 0;
        while (xml.FindElem("server"))
        {
            xml.IntoElem();
            
            printf("server idx: %d\n", iNumSvr);
            ++iNumSvr;

            iRet = LoadApiList_Api_AServer(xml);
            if (0 != iRet)
            {
                printf("LoadApiList_Api_AServer failed.\n");
                return -1;
            }
                
            xml.OutOfElem();// outof server
        }

        if (iNumSvr <= 0)
        {
            printf("server num:0 error\n");
            return -1;
        }
        else
        {
            printf("server num: %d\n",iNumSvr);
        }
    }
    xml.OutOfElem();// outof server_list    

    return 0;
}

int LoadApiList(CMarkup &xml)
{
    int iRet = 0;
    // 复位整个文档
    xml.ResetPos();

    if (!xml.FindElem("APP_CONFIG"))
    {
        return -1;
    }
    //g_xml_gen.AddElem("APP_CONFIG");
    //g_xml_gen.IntoElem();

    xml.IntoElem();
    if (!xml.FindElem("api_list"))
    {
        return -1;
    }
    //g_xml_gen.AddElem("api_list");
    //g_xml_gen.IntoElem();
    
    xml.IntoElem();
    if (!(xml.FindElem("qos") &&
        xml.FindElem("taskserver") &&
        xml.FindElem("api")
        ))
    {
        printf("find APP_CONFIG->api_list->[qos, taskserver, api] failed.\n");
        return -1;
    }


    xml.ResetMainPos();
    
    xml.FindElem("qos");
    printf("qos: rebuild_internal: %s, rebuild_max: %s, need_max_queue_protect:" 
            "%s, weight_by_avg_time: %s\n",
            xml.GetAttrib("rebuild_internal").c_str(),
            xml.GetAttrib("rebuild_max").c_str(),
            xml.GetAttrib("need_max_queue_protect").c_str(),
            xml.GetAttrib("weight_by_avg_time").c_str()
            );
    //g_xml_gen.AddElem("qos");
    //g_xml_gen.AddAttrib("rebuild_internal", xml.GetAttrib("rebuild_internal").c_str())
    xml.FindElem("qos");
    printf("qos: rebuild_internal: %s, rebuild_max: %s, need_max_queue_protect:" 
            "%s, weight_by_avg_time: %s\n",
            xml.GetAttrib("rebuild_internal").c_str(),
            xml.GetAttrib("rebuild_max").c_str(),
            xml.GetAttrib("need_max_queue_protect").c_str(),
            xml.GetAttrib("weight_by_avg_time").c_str()
            );
 
    xml.FindElem("taskserver");
    printf("server_num: %s\n", xml.GetAttrib("server_num").c_str());    

    int iNumApi = 0;
    while (xml.FindElem("api"))
    {
        printf("\napi idx: %d\n", iNumApi);
        ++iNumApi;
        xml.IntoElem();

        iRet = LoadApiList_AApi(xml);
        if(0 != iRet)
        {
            printf("LoadApiList_AApi(xml) failed.\n");
            return -1;
        }
        
        xml.OutOfElem();// outof api
    }
    printf("iNumApi : %d\n", iNumApi);
    
    
    return 0;

}

int LoadRoleList_ARole(CMarkup &xml)
{
    if (!(xml.FindElem("id") && 
          xml.FindElem("name") &&
          xml.FindElem("desc") &&
          xml.FindElem("api_list"))
        )
    {
        printf("find APP_CONFIG->role_list->role->[id, name, desc, api_list] failed.\n");
        return -1;
    }

    xml.ResetMainPos();
    xml.FindElem("id");
    printf("id: %s", xml.GetData().c_str());

    xml.FindElem("name");
    printf("name: %s", xml.GetData().c_str());

    xml.FindElem("desc");
    printf("desc: %s", xml.GetData().c_str());

    xml.FindElem("api_list");
    xml.IntoElem();
    {
        int iNumApi = 0;
        while (xml.FindElem("api"))
        {
            ++iNumApi;
            xml.IntoElem();
            if (!xml.FindElem("name"))
            {
                printf("find APP_CONFIG->role_list->role->api_list->api->[name] failed.\n");
                return -1;
            }
            printf("name: %s\n", xml.GetData().c_str());            
            xml.OutOfElem();// out of api
        }
        if (iNumApi <= 0)
        {
            printf("find APP_CONFIG->role_list->role->api_list->[api] failed.\n");
            return -1;
        }
    }
    xml.OutOfElem();  // out of api_list   

    return 0;
}

int LoadRoleList(CMarkup &xml)
{
    int iRet = 0;
    // 复位整个文档
    xml.ResetPos();

    if (!xml.FindElem("APP_CONFIG"))
    {
        return -1;
    }

    xml.IntoElem();
    if (!xml.FindElem("role_list"))
    {
        return -1;
    }

    xml.IntoElem();
    int iNumRole = 0;
    while (xml.FindElem("role"))
    {
        ++iNumRole;
        printf("role idx: %d\n", iNumRole);
        
        xml.IntoElem();

        iRet = LoadRoleList_ARole(xml);
        if (0 != iRet )
            return -1;

        xml.OutOfElem(); // out of role
    }

    if (iNumRole <= 0)
    {
        printf("find APP_CONFIG->role_list->[role] failed.\n");
        return -1;
    }
    else
    {
        printf("role num: %d", iNumRole);
    }

    return 0;
}

int main()
{
    CMarkup xml;
    string str = "api_config.xml.taskserver.nemo";
    //string str = "api_config.xml";
    if (!xml.Load(str.c_str()))
    {
        printf("load xml failed.\n");
        return -1;;
    }

    string str2 = "api_config_gen.xml";
    g_xml_gen.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");

#if 1
    printf("\n===============================================\n");
    printf("LoadApiList\n");    
    if (0 != LoadApiList(xml))
    {
        printf("load apilist failed. \n");
        return -1;
    }
#endif    

#if 0
    printf("\n\n");
    printf("LoadRoleList\n");    
    if (0 != LoadRoleList(xml))
    {
        printf("load rolelist failed. \n");
        return -1;
    }    
#endif    

#if 0
    printf("\n\n");
    printf("LoadAppList\n");    
    if (0 != LoadAppList(xml))
    {
        printf("load applist failed. \n");
        return -1;
    }
#endif    


    return 0;
        
}

#if 0
            if (!(xml.FindElem("ip") &&
                xml.FindElem("weight") &&
                xml.FindElem("max_queue_req"))
                )
            {
                printf("find APP_CONFIG->api_list->api->server_list"
                        "->server->[ip, , desc, server_list] failed.\n");
                return -1;
            }
            
            xml.ResetMainPos();
#endif

