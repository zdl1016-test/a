--m = require "print_tb"
require "print_md"
--nemo_module.print_tb({})
--table.foreach(nemo_module._M, print)
a = {}
--table.foreach(getmetatable(a), print)

--a = {}
--m.print_tb(a)
