--b = {}
a = {a = {1,2}}
a1 = a.a
table.insert(a1, 3)
table.foreach(a.a, print)
