local mt = {}
mt.__index={1,2,3,4,5,6,7,8,9,10}

local a = {3,2,1}
setmetatable(a, mt);

-- 通过rawget访问table, 跳过元表
print (rawget(a, 4))

-- 通过元表访问
print (a[4])
