d = require "print_tb"

--d.print_tb(debug.getinfo(d.print_tb))
--print"-------------"
--d.print_tb(debug.getinfo(1)) -- 得到当前chunk的自省

print"-------------"
local function log()
    d.print_tb(debug.getinfo(2)) -- 得到当前chunk的自省
end

function foo2()
log()
end

foo2()
