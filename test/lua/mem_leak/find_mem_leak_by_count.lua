-- 思路:
-- 采用广度优先的方式从_G遍历全部的TABLE, 统计每个TABLE的元素个数.
-- 排序输出元素最多的100个TABLE, 如果存在内存泄露, 那么泄露的对象一定在在TOP 100里面:)
--
--

-- 模拟构造一些大小不同的table
a = {}
b = {}
c = {1,2,3,4,5,6,7,8}
d = {}
for i=1,10000 do
    table.insert(d, i)
end
e = {[a] = 'a', b = b, c = c, [d] = d}
f = {1,2,{1,2, {1,2,e}}}


all_table = {}
setmetatable(all_table, {__mode='k'})
all_table[all_table] = false

table.foreach (all_table, print)

max_depth = 0
depth = 0
depth_path = {}
function breadth_first_search(root, name)
    if type(root) ~= 'table' then
        return
    end

    -- 避免交叉引用导致的死循环
    if all_table[root] then
        return
    end

    name = name or 'nil'

    depth = depth + 1
    if max_depth < depth then max_depth = depth end

    -- 如果树很深, 也是很恐怖的事情
    -- 不过链表数据结构会突破此限制...
    if depth > 100 then
        print ('!!!warning!!! depth over 100')
    end

    -- 统计root的元素个数
    local count = 0
    for k, v in pairs (root) do
        count = count + 1
    end
    all_table[root] = {count = count, name = name}

    -- 遍历子元素
    for k, v in pairs (root) do
        --breadth_first_search(k) -- key也有可能是table哦
        --table.insert(depth_path, k)
        local v_name = tostring(k)
        --if type(k) == 'table' then 
            --v_name = 'tb'
        --end
        v_name = string.format("%s_[%s]", name, v_name)
        --breadth_first_search(v, name .. "_" .. v_name)
        breadth_first_search(v, v_name)
        --table.remove(depth_path, k)
    end

    depth = depth - 1
    return
end

function p_table(t)
    if type(t) ~= 'table' then 
        return
    end

    for k, v in pairs (t) do
        print (k, v)
        p_table(v)
    end
end

print ("a:")
breadth_first_search(a)
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()

all_table = {[all_table] = false}
print ("b:")
breadth_first_search(b)
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()
    
all_table = {[all_table] = false}
print ("c:")
breadth_first_search(c)
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()

all_table = {[all_table] = false}
print ("d:")
breadth_first_search(d)
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()

all_table = {[all_table] = false}
print ("e:")
breadth_first_search(e)
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()

all_table = {[all_table] = false}
print ("f:")
breadth_first_search(f)
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()

all_table = {[all_table] = false}
print ("G:")
breadth_first_search(_G, '_G')
table.foreach(all_table, print)
print ("max_depth : ", max_depth)
print ()
p_table(all_table)

package.path = package.path .. ';/home/nemo/code/test/lua/heap_sort/?.lua;'
Heap = require "heap_sort"

function get_top_count(t)

    local top_count = 3

    -- 利用小顶堆找最大的topN个元素
    local min_heap = Heap()
    for k, v in pairs (t) do
        if v then
            -- 先筹齐N个元素
            if min_heap:get_size() < top_count then
                min_heap:insert(v.count, v.name)
            else
                -- 如果新元素还没有堆内最小的元素大, 那么直接忽略, 否则插入堆中
                if v.count > min_heap:get_min() then
                    min_heap:delete_min()
                    min_heap:insert(v.count, v.name)
                end
            end
        end
    end

    local result = {}
    while min_heap:get_size() > 0 do
        local node = {}
        node.name, node.count = min_heap:get_min()
        print (node.name, node.count)
        table.insert(result, node)
        min_heap:delete_min()
    end
end

get_top_count(all_table)

--test_heap = Heap()
--test_heap:insert( 84, "blah")
--test_heap:insert( 32, "hddfhh")
--test_heap:insert( 66, "hfhhfd")
--test_heap:insert( 77, "fjjjf")
--test_heap:insert( 2, "min")
--test_heap:insert( 3, "new min")
--test_heap:insert( 4, "fggjjg")

--test_heap:print_heap()
--print(test_heap:get_min())

--test_heap:delete_min()

--test_heap:print_heap()
--print(test_heap:get_min()) 
