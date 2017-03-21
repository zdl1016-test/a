tb = {}

table.insert(tb, {1,1});

function printtb(t)
	for k, v in pairs(t) do
		print (k, v)
	end
end

printtb(tb);
