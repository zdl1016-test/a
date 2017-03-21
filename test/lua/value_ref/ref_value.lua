--测试函数传参是传引用还是传值
md = require "print_tb"

-- 简单变量, 传值
a = 2
foo = function (a) a = 3 end
foo (a)
print (a)

-- 简单变量, 传值
a = "aaa" 
foo = function (a) a = "ssss" end
foo (a)
print (a)

-- table变量, 传引用 但是如果直接把table赋值其他table,不影响原table!!!
a = {1,2,3}
foo = function (a) print("in fun", a) a = {4,5,6} print("in fun", a) end
foo (a)
print (a)
md.print_tb (a)

-- table变量, 传引用
a = {1,2,3}
foo = function (a) print("in fun", a) a[#a+1] = 4  print("in fun", a) end
foo (a)
print (a)
md.print_tb (a)

