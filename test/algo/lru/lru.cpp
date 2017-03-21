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
#include <string>
#include <fstream>

using namespace std;
using namespace __gnu_cxx;

#ifndef foreach
#define foreach(container,it) \
            for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

struct DataValue
{
    string ss;
    int appid;
    string Info()
    {
        char buf[128];
        snprintf(buf, sizeof(buf), "[ss:%s, appid:%d]", ss.c_str(), appid);
        return buf;
    }
};

int MakeValue(int appid, DataValue& v)
{
    v.ss = "hello";
    v.appid = appid;
    return 0;
}

typedef int (*make_value_pt)(int, DataValue&);

class CLRU
{
public:
    CLRU(make_value_pt fn, int cache_size):
        _fn_make_value(fn),
        CACHE_SIZE(cache_size),
        m_hit_num(0),
        m_miss_num(0)
    {}

public:
    void get(int appid, DataValue& dv)
    {
        // 如果key已经存在, 把key移动到队tail
        if (key_map.find(appid) != key_map.end())
        {
            key_lru_list.splice(key_lru_list.end(), key_lru_list, key_map[appid].second);
            dv = key_map[appid].first;
            m_hit_num ++;
        }
        // if not exiest
        else
        {
            // if cash size is full, evict
            if (key_lru_list.size() >= (unsigned int)CACHE_SIZE)
            {
                key_map.erase(key_lru_list.front());
                key_lru_list.erase(key_lru_list.begin());

            }

            // make a value for key
            _fn_make_value(appid, dv);

            // insert
            key_lru_list.push_back(appid);
            key_map[appid] = pair<DataValue, list<int>::iterator>(dv, --key_lru_list.end());
            m_miss_num ++;
        }
    }

    // just for debug
    void output()
    {
        foreach(key_lru_list, it)
        {
            int appid = *it;
            printf("appid:%u, dv:%s\n", 
                    appid, key_map[appid].first.Info().c_str());
        }
    }

    void output_hit_info()
    {
        printf("cache_size:%d, req_num:%d, hit_num:%d, miss_num:%d, hit_radio:%5.3f\n",
                CACHE_SIZE, m_hit_num+m_miss_num, m_hit_num, m_miss_num, (float)m_hit_num/(m_hit_num+m_miss_num));
    }

    void reset()
    {
        key_lru_list.clear();
        key_map.clear();
    }

public:
    list<int>           key_lru_list;
    hash_map<int, pair<DataValue, list<int>::iterator> > key_map;

    make_value_pt       _fn_make_value;
    int                 CACHE_SIZE;
    int                 m_hit_num;  // 命中的次数
    int                 m_miss_num; // 没有命中的次数
};

void load_appids(vector<uint32_t> &appids);
int main()
{

    DataValue dv;

    vector<uint32_t> appids;
    load_appids(appids);

    printf("appids:%u\n", appids.size());

    for (int i=100; i<1000; i+=100)
    {
        CLRU lru(MakeValue, i);

        foreach (appids, it)
        {
            lru.get(*it, dv); 
        }

        lru.output_hit_info();
    }

    return 0;
}

void load_appids(vector<uint32_t> &appids)
{
    std::string line;
    std::fstream inFile;
    //inFile.open("/home/nemo/119/appids.txt", ios::in);
    inFile.open("/home/nemo/119/appids_lg.txt", ios::in);


    if (!inFile.is_open())
    {    
        return;
    }    

    while (getline(inFile, line))
    {    
        if (line.empty())
        {    
            continue;
        }    

        if (line[0] == '/' || line[0] == '#') 
        {    
            continue;
        }    

        u_int appid = strtoul(line.c_str(), 0, 10);
        appids.push_back(appid);
    }          

}
