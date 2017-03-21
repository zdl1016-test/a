-- 跟踪table的访问

local t_k = {} --把t存在代理table的t_k位置
local mt = {}

mt.__index = function (t, k) 
    print ("access to elem " .. tostring(k)) 
    return t[t_k][k] 
end

mt.__newindex = function (table, k, v) 
    print ("update elem [" ..  tostring(k) .. "] to value " .. tostring(v))
    table[t_k][k] = v
end

local function traceTable(t)
    local t_proxy = {} -- 用于代理的table
    t_proxy[t_k] = t
    setmetatable(t_proxy, mt)
    return t_proxy
end

local t = {}
local t2 = {}
print (t)
t = traceTable(t)
print (t)
t[2] = 3;
print (t[2])
