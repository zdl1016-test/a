--[[
JSON4Lua example script.
Demonstrates the simple functionality of the json module.
]]--
package.path = package.path .. ';../json/?.lua;'

json = require('json')


-- Object to JSON encode
--test = {
  --one='first',two='second',three={2,3,5}, [1010] = 1,
--}
test = {
    used_collect_times = 0,
    buildings = {
        [0] = {
            level = 1,
        },
        [1001] = {
            level = 1,
        },
        [1002] = {
            level = 1,
        },
    }
}

jsonTest = json.encode(test)

print('JSON encoded test is: ' .. jsonTest)

-- Now JSON decode the json string
result = json.decode(jsonTest)

print ("The decoded table result:")
table.foreach(result,print)

print ("The decoded table result.three")
table.foreach(result.buildings, print)
--print (result.buildings[1002].level)
--print (result.buildings["1002"].level)
