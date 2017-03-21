e = 10
function foo(a, b)
    return function ()
        --return e -- e不是upvalue
        return a -- a是个upvalue
    end
end

c = foo()

--print (c())

function print_upvalue(f)
    local index = 1
    while true do
        local name, value = debug.getupvalue(f, index)
        if not name then break end
        print (name, value)
        index = index + 1
    end
end

print_upvalue(c)
