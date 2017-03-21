local pairs = pairs
local print = print

module "nemo_module"

--print "print in nemo_module"
function print_tb(t)
    for k, v in pairs(t) do print (k, v) end
end

loadstring("function foo()return nil")

global_var_in_md= "im global var in md"

