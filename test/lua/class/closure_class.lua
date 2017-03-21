local function newObject (v)
    return function (action, v2)
        if action == "get" then return v
        elseif action == "set" then v = v2
        else error "invalid action"
        end
    end
end

local o = newObject(0)
print(o("get"))
o("set", 10)
print(o("get"))

local o2 = newObject(0)
print(o2("get"))
o2("set", 100)
print(o2("get"))

    

