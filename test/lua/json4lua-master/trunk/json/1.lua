function isdigit (str)
    if str == nil or type(str) ~= "string" then
        return false
    end
    local len = string.len(str)

    for i = 1, len do
        local ch = string.sub(str, i, i)
        if ch < '0' or ch > '9' then
            return false
        end
    end
    return true
end

print (isdigit("1111"))
print (isdigit())
print (isdigit("1112.0"))
a = {}
a[tonumber("11")] = 3;
print(a["11"])
