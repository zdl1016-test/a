-- 注意设置合适的环境变量
-- export LUA_CPATH="$C_PATH;/home/nemo/code/test/lua/lua_call_c/?.so;./?.so"
print (package.cpath)
array=require "array"
local a=array.new(10)
for i=1,10 do
	array.set(a, i, i/10)
end
print (array.size(a))
print (array.get(a, 2))

