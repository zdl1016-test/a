func, msg = loadfile("./inc.lua")
--inc_foo() -- 此时还没有inc_foo函数, 不能使用
print (func, msg)

func()
inc_foo()   -- 此时可以使用了

