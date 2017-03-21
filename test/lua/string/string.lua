d = require "print_tb"
s = 'abcABC'
print(string.byte(s, 1))
print(string.byte(s, 4))
print(string.byte(s, 1, 3))
a,b,c = string.byte(s, 1, 3)
print(a,b,c)

s = "nemo hello world !!"
words = {}
for w in string.gmatch(s, "%a+") do
    words[#words + 1] = w
end
d.print_tb(words)
