
--如果定义成局部,必须把require的结果保存才能用
local md = {}

-- 如果定义成全局变量,则require之后,可以直接使用md
--md = {}

function md.print_tb(t)
    for k, v in pairs(t) do print (k, v) end
end

print "print in print_tb.lua"
return md 
