md = require "print_tb"

Set = {}
local mt = {} --用作集合的元表

function Set.new (l)
    local set = {}
    setmetatable(set, mt) --绑定一个元表
    for _, v in ipairs(l) do 
        set[v] = true
    end
    return set
end

function Set.union(a, b)
    local res = Set.new{};
    for k in pairs (a) do res[k] = true end
    for k in pairs (b) do res[k] = true end
    return res
end

function Set.intersect(a, b)
    local res = Set.new{}; -- note, must use Set.new{} generate new table. 因为其设置了matetable
    for k in pairs (a) do res[k] = b[k] end
    return res
end

function Set.difference(a, b)
    local res = Set.new{};
    for k in pairs (a) do 
        if not b[k] then res[k] = true end
    end
    return res
end

function Set.pow(set, pow)
    local res = Set.new{};
    for k in pairs (set) do 
        res[k^pow] = true 
    end
    return res
end

function Set.mod(set, mod)
    local res = Set.new{};
    for k in pairs (set) do 
        res[k%mod] = true 
    end
    return res
end

function Set.tostring(set)
    local l = {}
    --把key转化为value
    for e in pairs (set) do l[#l+1] = e end
    return "{" .. table.concat(l, ", ") .. "}"
end

function Set.print(set)
    print(Set.tostring(set))
end

mt.__add = Set.union
mt.__mul = Set.intersect
mt.__sub = Set.difference
mt.__pow = Set.pow
mt.__mod = Set.mod
mt.__tostring= Set.tostring
mt.__metatable = "not your business"


s1 = Set.new({2,4,3,4})
s2 = Set.new({2,1,5,4})
s3 = s1 + s2 -- +操作会调用元表的__add成员(s1, s2)
s4 = s1 * s2 -- *操作会调用元表的__mul成员(s1, s2)
s5 = s1 - s2
s6 = s4 + s5
s7 = s1 ^ 2
s8 = s1 % 4

--s1的元表mt做了保护, 尝试替换元表会报错
--setmetatable(s1, {}) 
--s1的元表mt做了保护, 尝试获取元表会得到一个msg
--print(getmetatable(s1))

--md.print_tb(Set)
print "-------------"
Set.print(s1)
print "-------------"
Set.print(s2)
print "-------------"
Set.print(s3)
print "-------------"
Set.print(s4)
print "-------------s5"
Set.print(s5)
print "-------------s6"
Set.print(s6)
print "-------------s7"
Set.print(s7)
print "-------------s8"
Set.print(s8)
print "-------------s1"
Set.print(s1)
print(s1)
