function foo()
    print (debug.traceback())
end

function foo2()
    foo()
end

foo2()

print "done"
