s = '1234567890'
--print ("["..string.sub(s, 1, 30).."]")
print ("["..string.gsub(s, 1, 30).."]")
--print (string.sub(s, -2, -1))
