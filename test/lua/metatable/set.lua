md = require "print_tb"

Set = {}

function Set.new (l)
    local set = {}
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
    local res = Set.new{};
    for k in pairs (a) do res[k] = b[k] end
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

s1 = Set.new({2,4,3,4})
s2 = Set.new({2,1,5,4})
s3 = Set.union(s1, s2)
s4 = Set.intersect(s1, s2)

--md.print_tb(Set)
print "-------------"
md.print_tb(s1)
print "-------------"
md.print_tb(s2)
print "-------------"
md.print_tb(s3)
print "-------------"
md.print_tb(s4)

Set.print(s1)

