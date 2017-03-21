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

struct NumArray
{
	int size;
	double array[1];
};

static int l_newarray(lua_State *L)
{
	int num = luaL_checkint(L, 1);
	NumArray* a = (NumArray*)lua_newuserdata(L, sizeof(NumArray)+(num-1)*sizeof(double));
	a->size = num;
	return 1; // newuserdata now already on the stack
}

static int setarray(lua_State *L)
{
	NumArray * a = (NumArray*)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	double value = luaL_checknumber(L, 3);
	luaL_argcheck(L, a!=0, 1, "'array' expected");// 数字用来抛出错误提示用 bad argument #<narg> to <func> (<extramsg>)
	luaL_argcheck(L, index>0 && index<=a->size, 2, "invalid index");
	a->array[index-1] = value;

	return 0;
}

static int getarray(lua_State *L)
{
	NumArray * a = (NumArray*)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	luaL_argcheck(L, a!=0, 1, "'array' expected");// 数字用来抛出错误提示用 bad argument #<narg> to <func> (<extramsg>)
	luaL_argcheck(L, index>0 || index<=a->size, 1, "invalid index");
	lua_pushnumber(L, a->array[index-1]);
	return 1;
}

static int getarraysize(lua_State *L)
{
	NumArray * a = (NumArray*)lua_touserdata(L, 1);
	luaL_argcheck(L, a!=0, 1, "'array' expected");// 数字用来抛出错误提示用 bad argument #<narg> to <func> (<extramsg>)
	lua_pushnumber(L, a->size);

	return 1;
}

static const struct luaL_reg arraylib[] = {
	{"new", l_newarray},
	{"get", getarray},
	{"set", setarray},
	{"size", getarraysize},
	{0, 0}
};

int luaopen_array(lua_State *L)
{
	luaL_register(L, "array", arraylib);
	return 1;
}

}
