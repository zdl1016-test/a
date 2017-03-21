-- 注意设置合适的环境变量
-- export LUA_CPATH="$C_PATH;/home/nemo/code/test/lua/lua_call_c/?.so;./?.so"
print (package.cpath)
a=require "mylib"
print (a.mysin(3.14/6));
arr = a.mysplit("aaa,b,c", ",");
for k, v in ipairs(arr) do
    print (k, v)
end

arr2 = a.mysplit2("aaa,b,c", ",");
for k, v in ipairs(arr) do
    print (k, v)
end

c=a.myclosure()
print(c())
print(c())
print(c())
print(c())

c2=a.myclosure()
print(c2())
print(c2())
print(c2())
print(c2())
