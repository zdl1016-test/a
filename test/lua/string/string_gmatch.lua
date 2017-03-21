s = "t.x.y"

print(string.gmatch(s, "([%w_]+)(%.?)")) --gmatch 返回的是一个迭代器函数哦

-- 配合for使用, gmatch才有意义
for w, d in string.gmatch(s, "([%w_]+)(%.?)") do
    print (w, d)
end
