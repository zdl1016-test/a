s = "nemo = hello"
key, value = string.match(s, "([%w]+)%s*=%s*(%w+)")
print (key, value)

s = "battle.lua:33, foo()"
ret = string.match(s, "(.*battle.*):")
print (ret)

s = "plug_battle.lua:33, foo()"
ret = string.match(s, "(.*battle.*):")
print (ret)
