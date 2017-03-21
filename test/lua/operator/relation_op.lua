-- 测试关系操作符

print ("aa" == "aa") -- true
print ("aa" ~= "aa") -- false
print ("aa" > "aa")  -- false
print ("aa" >= "aa") -- true
print ("2" < "15") -- false 严格按照字符串比较 
print '---------'
print (11 == 11) -- true
print (11 ~= 11) -- false
print ("11" == 11)  -- false 类型不一样, 一定是false
print (22 >= 11) -- true

print '---------'
print ({} == {}) -- false
a = {};
b = a;
print (a == b)   -- true

