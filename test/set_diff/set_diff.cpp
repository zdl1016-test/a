#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

using namespace std;

#ifndef foreach
#define foreach(container,it) \
                for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

bool IsFileExist(const char* path)
{
    if (access(path, F_OK ) != 0)
    {
        return false;
    }
    return true;

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

void LoadFile(const char* file, set<string>& line_set)
{
    std::string line;
    std::fstream inFile;

    inFile.open(file, ios::in);

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

        line = trim(line);
        line_set.insert(line);
    }   
}


void print_set(set<string>& line_set)
{
    foreach (line_set, it)
    {
        printf("%s\n", (*it).c_str());
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("usage : %s left_file right_file\n", argv[0]);
        printf("note1: output elements in left_file but not in right_file\n" );
        printf("note2: auto trim blank char\n" );
        return -1;
    }

    char * l_file = argv[1];
    char * r_file = argv[2];
    
    if (!IsFileExist(l_file)) 
    {
        printf("can not find file : %s\n", l_file);
        return -1;
    }

    if (!IsFileExist(r_file)) 
    {
        printf("can not find file : %s\n", r_file);
        return -1;
    }

    set<string> l_set;
    set<string> r_set;

    set<string> diff_set;

    LoadFile(l_file, l_set);
    LoadFile(r_file, r_set);

    //printf("=========\n");
    //print_set(l_set);
    //printf("=========\n");
    //print_set(r_set);

    set_difference(l_set.begin(), l_set.end(), r_set.begin(), r_set.end(), inserter(diff_set, diff_set.begin()));

    //printf("=========\n");
    print_set(diff_set);
    return 0;

}

