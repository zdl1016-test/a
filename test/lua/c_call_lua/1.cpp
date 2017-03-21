#include <stdio.h>
#include <string.h>

extern  "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_loadfile(L, "config.lua") || lua_pcall(L, 0, 0, 0))
    {
        fprintf(stderr, "load config.lua failed,  msg:%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 0;
    }

    lua_getglobal(L, "width");
    printf("width:%g\n", lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_getglobal(L, "height");
    printf("heigth:%g\n", lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_getglobal(L, "bk");

    lua_getfield(L, -1, "r"); // 此时bk.r位于栈顶
    printf("r:%g\n", lua_tonumber(L, -1));
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "g"); // 此时bk.r位于栈顶
    printf("g:%g\n", lua_tonumber(L, -1));
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "b"); // 此时bk.r位于栈顶
    printf("b:%g\n", lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_getglobal(L, "fun");
    lua_pushnumber(L, 3);
    lua_pushnumber(L, 4);
    if (lua_pcall(L, 2, 1, 0) != 0)
    {
        fprintf(stderr, "call fun() failed,  msg:%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 0;
    }
    printf("result : %g\n", lua_tonumber(L, -1));
    lua_pop(L, 1);


    lua_close(L);

    printf("hello \n");
    return 0;
}
