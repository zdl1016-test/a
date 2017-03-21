#include <stdio.h>
#include <string.h>

extern  "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void stack_dump(lua_State *L);
int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    printf("===============\n");
    stack_dump(L);

    // 测试 lua_insert
    {
        lua_pushnumber(L, 1);
        lua_pushnumber(L, 2);
        lua_pushnumber(L, 3);
        lua_pushnumber(L, 4);
        printf("===============\n");
        stack_dump(L);

        lua_insert(L, 1);
        printf("===============\n");
        stack_dump(L);
        lua_settop(L, 0);
        stack_dump(L);
    }

    // 测试 lua_remove
    {
        lua_pushnumber(L, 1);
        lua_pushnumber(L, 2);
        lua_pushnumber(L, 3);
        lua_pushnumber(L, 4);
        printf("===============\n");
        stack_dump(L);

        lua_remove(L, 1);
        printf("===============\n");
        stack_dump(L);
        lua_settop(L, 0);
        stack_dump(L);
    }

    // 测试 lua_pushvalue
    {
        lua_pushnumber(L, 1);
        lua_pushnumber(L, 2);
        lua_pushnumber(L, 3);
        lua_pushnumber(L, 4);
        printf("===============\n");
        stack_dump(L);

        lua_pushvalue(L, 1);
        printf("===============\n");
        stack_dump(L);
        lua_settop(L, 0);
        stack_dump(L);
    }

    // 测试 lua_replace
    {
        lua_pushnumber(L, 1);
        lua_pushnumber(L, 2);
        lua_pushnumber(L, 3);
        lua_pushnumber(L, 4);
        printf("===============\n");
        stack_dump(L);

        lua_replace(L, 1);
        printf("===============\n");
        stack_dump(L);
        lua_settop(L, 0);
        stack_dump(L);
    }
    lua_close(L);

    return 0;
}
void stack_dump(lua_State *L)
{
    int top = lua_gettop(L);
    if (top == 0)
    {
        printf("empty\n");
        return;
    }

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
