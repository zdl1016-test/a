x = 4;
y = 5;
s = 'local foo = function (...) return arg[1]+arg[2] end; return foo'
--s2 = 'local foo = function (x,y) return x+y end; return foo'
foo2 = loadstring(s)()

table.foreach(_G, print)
print (foo2(1,13))
