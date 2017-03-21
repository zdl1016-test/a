#include <stdio.h>
#include <string.h>

extern  "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void stack_dump();
int main()
{
    stack_dump();
    return 0;
    char buf[512];
    int error;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_Number n;
    n = 0;

    buf[0] = 'a';
    buf[1] = '\0';
    buf[2] = 'b';
    lua_pushlstring(L, buf, 3);
    size_t len = 0;
    len = lua_objlen(L, -1);
    printf("objlen : %d\n", len);
        

    return 0;
    //lua_pushstring(L, "hello world");
    printf("isnumber: %d\n", lua_isnumber(L, -1));
    printf("isstring: %d\n", lua_isstring(L, -1));
    printf("istable: %d\n", lua_istable(L, -1));
    printf("isnil: %d\n", lua_isnil(L, -1));
    return 0;

    while(fgets(buf, sizeof(buf), stdin) != NULL)
    {
        error = luaL_loadbuffer(L, buf, strlen(buf), "line") || lua_pcall(L, 0, 0, 0);
        if (error)
        {
            fprintf(stderr, "%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }

    lua_close(L);

    printf("hello \n");
    return 0;
}
void stack_dump()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushlstring(L, "hello", 5);
    lua_pushstring(L, "world");
    lua_pushnumber(L, 99);
    lua_pushboolean(L, 3);
    lua_pushboolean(L, 0);
    lua_newtable(L);
    //lua_insert(L, 1);

    int top = lua_gettop(L);
    for (int i=1; i<=top; i++)
    {
        int t = lua_type(L, i);
        switch (t)
        {
        case LUA_TSTRING : 
            printf("%d : string[%s]\n", i, lua_tostring(L, i));
            break;
        case LUA_TNUMBER :
            printf("%d : number[%g]\n", i, lua_tonumber(L, i));
            break;
        case LUA_TNIL:
            printf("%d : nil\n", i);
            break;
        case LUA_TBOOLEAN:
            printf("%d : boolean[%d]\n", i, lua_toboolean(L, i));
            break;
        case LUA_TTABLE:
            printf("%d : table\n", i );
            break;
        default:
            printf("%d : unkown\n", i );
            break;
        }
    }
}
