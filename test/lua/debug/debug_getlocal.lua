function foo (a, b)
    local index = 1
    while true do
        local name, value = debug.getlocal(2, index)
        if not name  then break end
        print (name, value)
        index = index + 1
    end
end

local xx = 30
foo (10, 20)
