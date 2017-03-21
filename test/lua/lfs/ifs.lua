lfs = require "lfs"
dir = lfs.currentdir()
lfs.chdir("/usr")
dir = lfs.currentdir()
--lfs.
print (dir)
