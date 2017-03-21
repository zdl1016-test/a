-- 测试具有默认值的table

local mt = {}
local default_key = {}
-- 所有想具有默认值的table都共享的一个metatable
mt.__index = function (table, key) 
    -- 每个table的默认值存储在自己table里面,以default_key为key
    return table[default_key] 
end
local setDefault = function (t, default) 
    t[default_key] = default; 
    setmetatable(t, mt)
end

local a = {}
local b = {}

print(a.ss)
print(b.ss)

setDefault(a, 3);
setDefault(b, 4);

print(a.ss)
print(b.ss)
