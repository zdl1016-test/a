-- 将obj存储在obj_list里
local obj_list = {}
local b = {}
b.__mode = "v" -- value为弱引用

-- 将obj_list设置为弱表
setmetatable(obj_list, b)

id_count = 1

-- 创建obj
function create_obj()
    local obj = {}
    obj_list[id_count] = obj
    id_count = id_count + 1
    return obj
end

-- 测试obj
function test()
    local obj = create_obj()

    print "在obj的本地引用解除前:"
    collectgarbage()
    table.foreach (obj_list, print)

    print "在obj的本地引用解除后:"
    obj = nil
    collectgarbage()
    table.foreach (obj_list, print)
end

test()

