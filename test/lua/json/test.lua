JSON = (loadfile "json.lua")()

test = {
    used_collect_times = 0,
    buildings = {
        --[0] = {
            --level = 1,
        --},
        [1001] = {
            level = 1,
        },
        [1002] = {
            level = 1,
        },
    }
}

s = JSON:encode(test)
print (s)

arr2 = JSON:decode(s)
table.foreach(arr2, print)
table.foreach(arr2.buildings, print)
--print (arr2.buildings[1002])
--print (arr2.buildings["1002"])
