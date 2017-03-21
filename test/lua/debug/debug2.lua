
function trace(event, line)
    --local s = debug.getinfo(2).short_src
    --print(s .. ":" .. line)
    table.foreach(debug.getinfo(2), print)
end

--debug.sethook(print, "l")
trace(1,1)

--trace(nil, 3)
print("ss")
