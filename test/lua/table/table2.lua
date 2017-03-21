-- 测试table中的key是经过排序的, 循环遍历时, 并非你写入时的顺序
--


function printtb()
	for k, v in pairs(t) do
		print (k, v)
	end
end

-- 写入顺序是 1, 3, 2
t = {[1] = 1, [3] = 3, [2] = 2}

-- 遍历顺序是 1, 2, 3
printtb(t)
