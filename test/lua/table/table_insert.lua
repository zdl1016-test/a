d = require "print_tb"
a = {3,2,1}
table.insert(a, 1, 1)
print '------------'
d.print_tb(a);

table.insert(a, 4)
print '------------'
d.print_tb(a);

a[9] = 9;
print '------------'
d.print_tb(a);

table.insert(a, 4)
print '------------'
d.print_tb(a);

table.remove(a)
print '------------'
d.print_tb(a);
