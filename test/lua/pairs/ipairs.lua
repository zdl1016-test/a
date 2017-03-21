t = {}
--t[1] = 1;
t[2] = 2;
t[3] = 3;
t[9] = 9;

for k, v in ipairs(t) do
	print (k, v)
end

for k, v in pairs(t) do
	print (k, v)
end

