t = {}
t.m_xx = {}
--assert(not t.m_xx)
assert(not nil)
assert(not false)
--assert(not {})
t2 = {}
for k, v in pairs (t2) do
	xx[k] = v
end

a = 1
b = "2"
assert(false, a .. "!=" .. b)
