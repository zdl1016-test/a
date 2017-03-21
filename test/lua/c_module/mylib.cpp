#include <stdio.h>
#include <string.h>
#include <math.h>

extern  "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

extern "C" 
{


// 注意, 函数是static
static int l_sin(lua_State* L)
{
    double d = lua_tonumber(L, 1); // !!! 注意, 不是-1哦, 每个函数都有自己的私有的栈
    lua_pushnumber(L, sin(d));
    return 1; // 返回一个结果
}

// 注意, 函数是static
static int l_split(lua_State* L)
{
    const char *s = luaL_checkstring(L, 1); // 第一个入参
    const char *sep = luaL_checkstring(L, 2); // 第二个入参

    const char *e;
    int i = 1;
    lua_newtable(L);
    while ((e=strchr(s, *sep)) != NULL)
    {
        lua_pushlstring(L, s, e-s); // push value
        lua_rawseti(L, -2, i++); // i 是 key
        s = e+1;
    }

    lua_pushstring(L, s);
    lua_rawseti(L, -2, i);
    return 1;
}

// 注意, 函数是static
static int l_split2(lua_State* L)
{
    const char *s = luaL_checkstring(L, 1); // 第一个入参
    const char *sep = luaL_checkstring(L, 2); // 第二个入参

    const char *e;
    int i = 1;
    lua_newtable(L); // push table
    while ((e=strchr(s, *sep)) != NULL)
    {
        lua_pushnumber(L, i); // push key
        lua_pushlstring(L, s, e-s); // push value
        lua_settable(L, -3);
        s = e+1;
    }

    lua_pushnumber(L, i); // push key
    lua_pushstring(L, s);
    lua_settable(L, -3);
    return 1;
}

// 闭包的例子 upvalue就像c函数的静态变量
static int counter(lua_State *L)
{
    int val = lua_tointeger(L, lua_upvalueindex(1)); // 得到第一个upvalue
    lua_pushinteger(L, ++val); // 新的只压入栈,作为返回
    lua_pushvalue(L, -1); // 把栈顶复制一份再压入栈
    lua_replace(L, lua_upvalueindex(1)); // 弹出刚才复制的栈顶,赋值给upvalue保存下来
    return 1;
}

int newCounter(lua_State *L)
{
    lua_pushinteger(L, 0); //upvalue的初始化的值
    lua_pushcclosure(L, &counter, 1); // 1表示upvalue的数量, counter是操作闭包的函数
    return 1; // 返回新创建的闭包
}

// 本模块将要导出的函数列表
static const struct luaL_Reg mylib[] = {
    {"mysin", l_sin},
    {"mysplit", l_split},
    {"mysplit2", l_split2},
    {"myclosure", newCounter},
    {NULL, NULL}
};

// 该函数必须和最终生成的so的名字一致
int luaopen_mylib(lua_State *L)
{
    luaL_register(L, "mylib", mylib); // 注册本模块, 会保留一个table在栈中
    return 1; // 返回一个结果
}

}
