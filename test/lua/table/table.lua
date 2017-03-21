
function printtb(t)
	for k, v in pairs(t) do
		print (k, v)
	end
end

print "------------------------- 赋值的形式 -----------"

tb = {}

-- 字符串key
tb.aaa = "aaa"
tb["bbb"] = "bbb"
tb["1"] = "string key 1"

-- 整形key
tb[1] = "integer key 1"

-- 变量key
key = {};
tb[key] = "var key's value"
printtb(tb)

print "------------------------- 初始化的形式 -----------"
-- 字面字符串key以及整形key
tb2 = {aaa = "aaa", bbb = "bbb", [1] = "1"}
printtb(tb2)

print "------------------------- 初始化的形式 -----------"
-- 变量key
ccc = "xx"
tb3 = {[tb] = "aaa", ccc = "bbb", [ccc] = "bbb", [key] = "key's value"}
printtb(tb3)
