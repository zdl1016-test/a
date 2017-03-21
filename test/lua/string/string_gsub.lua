s = "hello world nemo!)(*&"
print(string.gsub(s, "[^0-9a-zA-Z]", "|"))
print(string.gsub(s, "[%W]", "|"))

s = "function(arg1@arg2) return nil end"

print(string.gsub(s, "@", ","))

s = [["function(arg1@arg2) return nil end"]]
print (s)
s = string.gsub(s, "\"", "")
print(s)
