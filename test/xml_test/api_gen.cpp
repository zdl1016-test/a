#include <stdio.h>
#include <string>
#include "Markup.h"

using namespace std;

int LoadAppList(CMarkup &xml)
{/*{{{*//*}}}*/
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
        ++iNumApp;
        printf("app idx: %d", iNumApp);
        
        xml.IntoElem();
        if (!(xml.FindElem("id") &&
            xml.FindElem("name") &&
            xml.FindElem("desc") &&
            xml.FindElem("appkey") &&
            xml.FindElem("ip_list") &&
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
        printf("id : %s ", xml.GetData().c_str());
        
        xml.FindElem("name");
        printf("name : %s ", xml.GetData().c_str());

        xml.FindElem("appkey");
        printf("appkey : %s ", xml.GetData().c_str());

        xml.FindElem("ip_list");

        int iNumIp = 0;
        xml.IntoElem();
        while (xml.FindElem("ip"))
        {
            printf("ip idx: %d", iNumIp);
            ++iNumIp;
            printf("ip: %s", xml.GetData().c_str());
        }
        xml.OutOfElem(); // outof ip_list                

        // 允许IP的个数为0
        if (iNumIp <= 0)
        {
            printf("ip num: 0");
        }
        else
        {
            printf("ip num: %d", iNumIp);
        }
        
        xml.OutOfElem();// outof app
        printf("\n");
    }

    if (iNumApp <= 0)
    {
        printf("app num: 0\n");
        return -1;
    }

    return 0;
}


int LoadApiList(CMarkup &xml)
{
    // 复位整个文档
    xml.ResetPos();

    if (!xml.FindElem("APP_CONFIG"))
    {
        return -1;
    }

    xml.IntoElem();
    if (!xml.FindElem("api_list"))
    {
        return -1;
    }

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

    xml.FindElem("taskserver");
    printf("server_num: %s\n", xml.GetAttrib("server_num").c_str());    

    int iNumApi = 0;
    while (xml.FindElem("api"))
    {
        printf("\napi idx: %d\n", iNumApi);
        ++iNumApi;
        xml.IntoElem();
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
        {
            xml.IntoElem();
            int iNumSvr = 0;
            while (xml.FindElem("server"))
            {
                printf("server idx: %d\n", iNumSvr);
                ++iNumSvr;
                xml.IntoElem();
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

                xml.OutOfElem();// outof server
            }
            xml.OutOfElem();// outof server_list

            if (iNumSvr <= 0)
            {
                printf("server num:0 error\n");
                return -1;
            }
            else
            {
                printf("server num:iNumSvr\n");
            }
        }
        xml.OutOfElem();// outof api
    }
    
    return 0;

}

int LoadRoleList(CMarkup &xml)
{
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
            xml.OutOfElem();
        }
        if (iNumApi <= 0)
        {
            printf("find APP_CONFIG->role_list->role->api_list->[api] failed.\n");
            return -1;
        }
        xml.OutOfElem();        
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
    string str = "api_config_gen.xml";
    xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    
    xml.AddElem("APP_CONFIG");
    xml.IntoElem();
    
    xml.AddElem("api_list");
    xml.IntoElem();
    
    xml.AddElem("qos");
    xml.AddAttrib("rebuild_internal", "20");
    xml.AddAttrib("rebuild_max", "100");
    xml.AddAttrib("need_max_queue_protect", "0");
    xml.AddAttrib("weight_by_avg_time", "1");

    xml.AddElem("taskserver");
    xml.AddAttrib("server_num", "1");
    
    //xml.OutOfElem();
    xml.Save("UserInfo.xml");
    
    
    return 0;
        
}
