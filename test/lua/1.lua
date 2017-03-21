a = {{1},{2},3,4,5}
--for i=1,#a do
    --if i==2 then
        --break
    --end
    --print i
--end
for k, v in pairs(a) do
    if type(v) == 'table' then
        table.foreach(v, print)
    else
        print (v)
    end
end

local xx = "s"
function foo (xx)
    print (xx)
end

foo("siis")
print (a)
