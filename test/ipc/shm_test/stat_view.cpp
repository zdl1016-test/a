#include "stat_simple.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

#ifndef foreach
#define foreach(container,it) \
        for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

string trim(const string& str);
void Tokenize(const string& src, const string& tok, vector<string>&v, bool empty_reserve=false);
int parse_conf(string stat_conf, map<int , string>& item_map);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage : %s shm_name stat_conf [inc:0, acc:1]\n", argv[0]);
        return -1;
    }

    int view_mode = 0; // default 显示每秒增量
    if (argc >=4 )
    {
        view_mode = atoi(argv[3]);
    }

    StatSimple stat(argv[1], 10, StatSimple::ENUM_WORK_MODE_CLI);

    if (stat.Init())
    {
        printf("stat init failed. msg : %s\n", stat.GetErrMsg());
        return -1;
    }

    map<int, string> item_map;
    parse_conf(argv[2], item_map);

    printf("====== parse conf result===============\n");
    foreach (item_map, it)
    {
        printf("%d \t:%s\n", it->first, it->second.c_str());
    }
    printf("====== parse conf result end===========\n");

    map<int, int> last_record_map;
    for(;;)
    {
        printf("==========================\n");
        foreach (item_map, it)
        {
            int index = it->first;
            if (view_mode == 0)
            {
                printf("%s \t:%d\n", it->second.c_str(), stat.Get(index) - last_record_map[index]);
            }
            else
            {
                printf("%s \t:%d\n", it->second.c_str(), stat.Get(index));
            }
            last_record_map[index] = stat.Get(index);
        }
        
        sleep(1);
    }

    return 0;
}
string trim(const string& str)
{
    string pat = " \r\n\t\0";
    string::size_type pos = str.find_first_not_of(pat);                                                                                                                          
    if (pos == string::npos)
    {   
        return ""; 
    }   
    string::size_type pos2 = str.find_last_not_of(pat);
    if (pos2 != string::npos)
    {   
        return str.substr(pos, pos2 - pos + 1); 
    }   
    return str.substr(pos);
}
void Tokenize(const string& src, const string& tok, vector<string>&v, bool empty_reserve)
{
    v.clear();
    size_t cur_pos = 0;
    size_t hit_pos = 0;
    size_t tok_len = strlen(tok.c_str());
    size_t src_len = src.length();

    while (cur_pos<=src_len)
    {   
        hit_pos = src.find(tok, cur_pos);
        if (hit_pos != string::npos)
        {   
            if (hit_pos!=cur_pos)
            {
                v.push_back(src.substr(cur_pos, hit_pos-cur_pos));
            }
            else
            {
                if (empty_reserve)
                    v.push_back("");
            }
            cur_pos = hit_pos + tok_len;
        }   
        else
        {   
            v.push_back(src.substr(cur_pos));
            break;
        }   
    }   
}  
int parse_conf(string stat_conf, map<int , string>& item_map)
{
    std::string line;
    std::fstream inFile;
    inFile.open(stat_conf.c_str(), ios::in);

    if (!inFile.is_open())
    {    
        printf("open file failed. file:%s\n", stat_conf.c_str());
        return -1;
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

        vector<string> segs; 
        Tokenize(line, ":", segs);
        if (segs.size() !=2)
        {
            continue;
        }

        int index = atoi(segs[0].c_str());
        string item_desc = trim(segs[1]);
        item_map[index] = item_desc;
    }          

    return 0;
}
